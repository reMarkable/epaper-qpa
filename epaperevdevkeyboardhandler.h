/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QFileSystemWatcher>
#include <QTimer>
#include <qobject.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QSocketNotifier;

namespace EpaperEvdevKeyboardMap {
const quint32 FileMagic = 0x514d4150; // 'QMAP'

enum InputFlavor {
    Windows,
    Apple
};

// A Mapping encodes a way to go from an evdev keycode (and set of modifiers) to a
// Qt key, and perhaps a special action, etc.
//
// Keys are "mapped" from evdev to Qt events, by checking the evdev's keycode against
// that of the mapping, along with checking the currently-applied modifiers against that of the mapping.
struct Mapping
{
    // The key reported by the kernel, used to match events.
    quint16 keycode;

    // This is used populate QKeyEvent::text, if not 0xffff (compose related?)
    quint16 unicode;

    // Maps to Qt::Key
    quint32 qtcode;

    // Modifiers for the key, used to match events.
    // If 0, then this is a plain key mapping.
    // Otherwise, the modifiers on the mapping must be set for this mapping to match
    quint16 modifiers;

    // See the Flags enum below
    quint8 flags;

    // If IsSystem, then it's the special action for this mapping. See the System enum below.
    // If IsModifier, then the special value is used to alter the currently pressed modifiers.
    quint16 special;
};

inline auto tieMapping(Mapping const &mapping)
{
    return std::tie(mapping.keycode, mapping.unicode, mapping.qtcode, mapping.modifiers, mapping.flags, mapping.special);
}

inline bool operator==(Mapping const &a, Mapping const &b)
{
    return tieMapping(a) == tieMapping(b);
}

inline bool operator!=(Mapping const &a, Mapping const &b)
{
    return tieMapping(a) != tieMapping(b);
}

enum Flags {
    IsDead = 0x01,
    IsLetter = 0x02,
    IsModifier = 0x04,
    IsSystem = 0x08,
    IsCapsLockException = 0x10,
};

enum System {
    SystemConsoleFirst = 0x0100,
    SystemConsoleMask = 0x007f,
    SystemConsoleLast = 0x017f,
    SystemConsolePrevious = 0x0180,
    SystemConsoleNext = 0x0181,
    SystemReboot = 0x0200,
    SystemZap = 0x0300
};

struct Composing
{
    quint16 first;
    quint16 second;
    quint16 result;
};

enum Modifiers {
    ModPlain = 0x00,
    ModShift = 0x01,
    ModAltGr = 0x02,
    ModControl = 0x04,
    ModAlt = 0x08,
    ModShiftL = 0x10,
    ModShiftR = 0x20,
    ModCtrlL = 0x40,
    ModCtrlR = 0x80,
    ModMeta = 0x100,
};
}

class EpaperEvdevFdContainer
{
    int m_fd;
    Q_DISABLE_COPY_MOVE(EpaperEvdevFdContainer);

public:
    explicit EpaperEvdevFdContainer(int fd = -1) noexcept :
        m_fd(fd) { }
    ~EpaperEvdevFdContainer() { reset(); }

    int get() const noexcept { return m_fd; }

    int release() noexcept
    {
        int result = m_fd;
        m_fd = -1;
        return result;
    }
    void reset() noexcept;
};

class EpaperEvdevKeyboardHandler : public QObject
{
public:
    EpaperEvdevKeyboardHandler(const QString &device, EpaperEvdevFdContainer &fd, bool disableZap);
    ~EpaperEvdevKeyboardHandler();

    enum class EpaperEvdevInputLocale {
        Denmark,
        Finland,
        France,
        Germany,
        Norway,
        Spain,
        Sweden,
        UnitedKingdom,
        UnitedStates
    };

    enum KeycodeAction {
        None = 0,

        CapsLockOff = 0x01000000,
        CapsLockOn = 0x01000001,
        NumLockOff = 0x02000000,
        NumLockOn = 0x02000001,
        ScrollLockOff = 0x03000000,
        ScrollLockOn = 0x03000001,

        Reboot = 0x04000000,

        PreviousConsole = 0x05000000,
        NextConsole = 0x05000001,
        SwitchConsoleFirst = 0x06000000,
        SwitchConsoleLast = 0x0600007f,
        SwitchConsoleMask = 0x0000007f
    };

    static std::unique_ptr<EpaperEvdevKeyboardHandler> create(const QString &device,
                                                              const QString &specification);

    static Qt::KeyboardModifiers toQtModifiers(quint16 mod)
    {
        Qt::KeyboardModifiers qtmod = Qt::NoModifier;

        if (mod & (EpaperEvdevKeyboardMap::ModShift | EpaperEvdevKeyboardMap::ModShiftL | EpaperEvdevKeyboardMap::ModShiftR)) {
            qtmod |= Qt::ShiftModifier;
        }
        if (mod & (EpaperEvdevKeyboardMap::ModControl | EpaperEvdevKeyboardMap::ModCtrlL | EpaperEvdevKeyboardMap::ModCtrlR)) {
            qtmod |= Qt::ControlModifier;
        }
        if (mod & EpaperEvdevKeyboardMap::ModAlt) {
            qtmod |= Qt::AltModifier;
        }
        if (mod & EpaperEvdevKeyboardMap::ModMeta) {
            qtmod |= Qt::MetaModifier;
        }

        return qtmod;
    }

    void resetKeymap();

    void readKeycode();
    KeycodeAction processKeycode(quint16 keycode, bool pressed, bool autorepeat);
    void setCapsLockEnabled(bool enabled);
    void setInputFlavor(EpaperEvdevKeyboardMap::InputFlavor flavor);

private:
    void processKeyEvent(int nativecode, int unicode, int qtcode,
                         Qt::KeyboardModifiers modifiers, bool isPress, bool autoRepeat);
    void switchLed(int, bool);

    template <typename LocaleType>
    void populateKeymap();

    QString m_device;
    EpaperEvdevFdContainer m_fd;
    QSocketNotifier *m_notify;

    // keymap handling
    quint16 m_modifiers;
    quint8 m_locks[3];

    bool m_no_zap;

    bool m_didLoadKeymap = false;
    EpaperEvdevKeyboardMap::Mapping *m_keymap;
    int m_keymap_size;

    std::vector<std::pair<quint16, quint16>> m_capsLockException;
    QFileSystemWatcher m_watcher;
    QString m_firmwareLang;
    EpaperEvdevInputLocale m_prevLocale;
    EpaperEvdevKeyboardMap::InputFlavor m_flavor;

private slots:
    void onSettingsChanged();
};

QT_END_NAMESPACE
