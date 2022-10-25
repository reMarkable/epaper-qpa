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

#include "epaperevdevkeyboardhandler.h"

#include <optional>
#include <qplatformdefs.h>

#include <QCoreApplication>
#include <QFile>
#include <QLoggingCategory>
#include <QSocketNotifier>
#include <QStringList>
#include <private/qcore_unix_p.h>
#include <qpa/qwindowsysteminterface.h>

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/private/qinputdevicemanager_p.h>

#ifdef Q_OS_FREEBSD
#include <dev/evdev/input.h>
#else
#include <linux/input.h>
#endif

#ifndef input_event_sec
#define input_event_sec time.tv_sec
#endif

#ifndef input_event_usec
#define input_event_usec time.tv_usec
#endif

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(EpaperEvdevKeyboardLog, "rm.epaperkeyboardhandler")
Q_LOGGING_CATEGORY(EpaperEvdevKeyboardMapLog, "rm.epaperkeyboardhandler.map")

namespace
{
    QSettings const qtSettings("remarkable", "xochitl");
    QString const sysfsLangFile("/sys/pogo/status/lang");

    using EpaperEvdevInputLocale = EpaperEvdevKeyboardHandler::EpaperEvdevInputLocale;

    // Reads the sysfs file for the lang value set by the keyboard firmware.
    // Only to be used as fallback; QT settings should have precedence over this value.
    std::optional<EpaperEvdevInputLocale> determineKeymapFirmware()
    {
        QFile file(sysfsLangFile);
        if (!file.open(QIODevice::ReadOnly)) {
            // No keyboard attached means we fall back to the (unaltered) US keymap.
            qCWarning(EpaperEvdevKeyboardLog) << "Failed to open pogo lang status: " << file.errorString();
            return {};
        }

        QByteArray langCode = file.readAll();
        qCDebug(EpaperEvdevKeyboardLog) << "Read a langCode of " << langCode;
        if (langCode.isEmpty()) {
            // This shouldn't happen, but if it does, try to behave sensibly.
            return {};
        }

        // TODO: What other values can be read from the firmware?
        if (langCode == "ES") {
            return EpaperEvdevInputLocale::Spain;
        } else if (langCode == "NO") {
            return EpaperEvdevInputLocale::Norway;
        } else if (langCode == "UK") {
            return EpaperEvdevInputLocale::UnitedKingdom;
        } else if (langCode == "US") {
            return EpaperEvdevInputLocale::UnitedStates;
        }

        return {};
    }

    // Reads the input locale setting, set from "Type Folio - Keyboard language" in settings menu.
    std::optional<EpaperEvdevInputLocale> determineKeymapSettings()
    {
        QString locale = qtSettings.value("InputLocale").toString();

        if (locale == "no_DK") {
            return EpaperEvdevInputLocale::Denmark;
        } else if (locale == "no_NO") {
            return EpaperEvdevInputLocale::Norway;
        } else if (locale == "no_SV") {
            return EpaperEvdevInputLocale::Sweden;
        } else if (locale == "fi_FI") {
            return EpaperEvdevInputLocale::Finland;
        } else if (locale == "en_UK") {
            return EpaperEvdevInputLocale::UnitedKingdom;
        } else if (locale == "en_US") {
            return EpaperEvdevInputLocale::UnitedStates;
        } else if (locale == "es_ES") {
            return EpaperEvdevInputLocale::Spain;
        } else if (locale == "fr_FR") {
            return EpaperEvdevInputLocale::France;
        } else if (locale == "de_DE") {
            return EpaperEvdevInputLocale::Germany;
        }

        return {};
    }
}

void EpaperEvdevFdContainer::reset() noexcept
{
    if (m_fd >= 0)
        qt_safe_close(m_fd);
    m_fd = -1;
}

EpaperEvdevKeyboardHandler::EpaperEvdevKeyboardHandler(const QString &device, EpaperEvdevFdContainer &fd, bool disableZap, bool enableCompose, const QString &keymapFile) :
    m_device(device), m_fd(fd.release()), m_notify(nullptr),
    m_modifiers(0), m_composing(0), m_dead_unicode(0xffff),
    m_no_zap(disableZap), m_do_compose(enableCompose),
    m_keymap(0), m_keymap_size(0), m_keycompose(0), m_keycompose_size(0)
{
    qCDebug(EpaperEvdevKeyboardLog) << "Create keyboard handler with for device" << device;

    // Watch the xochitl configuration file for changes in locale settings.
    m_watcher.addPath(qtSettings.fileName());
    this->onSettingsChanged();
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &EpaperEvdevKeyboardHandler::onSettingsChanged);

    setObjectName(QLatin1String("LinuxInput Keyboard Handler"));

    memset(m_locks, 0, sizeof(m_locks));

    if (keymapFile.isEmpty() || !loadKeymap(keymapFile))
        unloadKeymap();

    // socket notifier for events on the keyboard device
    m_notify = new QSocketNotifier(m_fd.get(), QSocketNotifier::Read, this);
    connect(m_notify, &QSocketNotifier::activated, this, &EpaperEvdevKeyboardHandler::readKeycode);
}

EpaperEvdevKeyboardHandler::~EpaperEvdevKeyboardHandler()
{
    unloadKeymap();
}

std::unique_ptr<EpaperEvdevKeyboardHandler> EpaperEvdevKeyboardHandler::create(const QString &device,
                                                                               const QString &specification,
                                                                               const QString &defaultKeymapFile)
{
    qCDebug(EpaperEvdevKeyboardLog, "Try to create keyboard handler for \"%ls\" \"%ls\"",
            qUtf16Printable(device), qUtf16Printable(specification));

    QString keymapFile = defaultKeymapFile;
    int repeatDelay = 400;
    int repeatRate = 80;
    bool disableZap = false;
    bool enableCompose = false;
    int grab = 0;

    const auto args = specification.splitRef(QLatin1Char(':'));
    for (const QStringRef &arg : args) {
        if (arg.startsWith(QLatin1String("keymap=")))
            keymapFile = arg.mid(7).toString();
        else if (arg == QLatin1String("disable-zap"))
            disableZap = true;
        else if (arg == QLatin1String("enable-compose"))
            enableCompose = true;
        else if (arg.startsWith(QLatin1String("repeat-delay=")))
            repeatDelay = arg.mid(13).toInt();
        else if (arg.startsWith(QLatin1String("repeat-rate=")))
            repeatRate = arg.mid(12).toInt();
        else if (arg.startsWith(QLatin1String("grab=")))
            grab = arg.mid(5).toInt();
    }

    qCDebug(EpaperEvdevKeyboardLog, "Opening keyboard at %ls", qUtf16Printable(device));

    EpaperEvdevFdContainer fd(qt_safe_open(device.toLocal8Bit().constData(), O_RDONLY | O_NDELAY, 0));
    if (fd.get() >= 0) {
        ::ioctl(fd.get(), EVIOCGRAB, grab);
        if (repeatDelay > 0 && repeatRate > 0) {
            int kbdrep[2] = { repeatDelay, repeatRate };
            ::ioctl(fd.get(), EVIOCSREP, kbdrep);
        }

        return std::unique_ptr<EpaperEvdevKeyboardHandler>(new EpaperEvdevKeyboardHandler(device, fd, disableZap, enableCompose, keymapFile));
    } else {
        qErrnoWarning("Cannot open keyboard input device '%ls'", qUtf16Printable(device));
        return nullptr;
    }
}

void EpaperEvdevKeyboardHandler::switchLed(int led, bool state)
{
    qCDebug(EpaperEvdevKeyboardLog, "switchLed %d %d", led, int(state));

    struct timeval tv;
    ::gettimeofday(&tv, 0);
    struct ::input_event led_ie;
    led_ie.input_event_sec = tv.tv_sec;
    led_ie.input_event_usec = tv.tv_usec;
    led_ie.type = EV_LED;
    led_ie.code = led;
    led_ie.value = state;

    qt_safe_write(m_fd.get(), &led_ie, sizeof(led_ie));
}

void EpaperEvdevKeyboardHandler::readKeycode()
{
    struct ::input_event buffer[32];
    int n = 0;

    forever {
        int result = qt_safe_read(m_fd.get(), reinterpret_cast<char *>(buffer) + n, sizeof(buffer) - n);

        if (result == 0) {
            qWarning("evdevkeyboard: Got EOF from the input device");
            return;
        } else if (result < 0) {
            if (errno != EINTR && errno != EAGAIN) {
                qErrnoWarning("evdevkeyboard: Could not read from input device");
                // If the device got disconnected, stop reading, otherwise we get flooded
                // by the above error over and over again.
                if (errno == ENODEV) {
                    delete m_notify;
                    m_notify = nullptr;
                    m_fd.reset();
                }
                return;
            }
        } else {
            n += result;
            if (n % sizeof(buffer[0]) == 0)
                break;
        }
    }

    n /= sizeof(buffer[0]);

    for (int i = 0; i < n; ++i) {
        if (buffer[i].type != EV_KEY)
            continue;

        quint16 code = buffer[i].code;
        qint32 value = buffer[i].value;

        EpaperEvdevKeyboardHandler::KeycodeAction ka;
        ka = processKeycode(code, value != 0, value == 2);

        switch (ka) {
        case EpaperEvdevKeyboardHandler::CapsLockOn:
        case EpaperEvdevKeyboardHandler::CapsLockOff:
            switchLed(LED_CAPSL, ka == EpaperEvdevKeyboardHandler::CapsLockOn);
            break;

        case EpaperEvdevKeyboardHandler::NumLockOn:
        case EpaperEvdevKeyboardHandler::NumLockOff:
            switchLed(LED_NUML, ka == EpaperEvdevKeyboardHandler::NumLockOn);
            break;

        case EpaperEvdevKeyboardHandler::ScrollLockOn:
        case EpaperEvdevKeyboardHandler::ScrollLockOff:
            switchLed(LED_SCROLLL, ka == EpaperEvdevKeyboardHandler::ScrollLockOn);
            break;

        default:
            // ignore console switching and reboot
            break;
        }
    }
}

void EpaperEvdevKeyboardHandler::processKeyEvent(int nativecode, int unicode, int qtcode,
                                                 Qt::KeyboardModifiers modifiers, bool isPress, bool autoRepeat)
{
    if (!autoRepeat)
        QGuiApplicationPrivate::inputDeviceManager()->setKeyboardModifiers(EpaperEvdevKeyboardHandler::toQtModifiers(m_modifiers));

    QWindowSystemInterface::handleExtendedKeyEvent(0, (isPress ? QEvent::KeyPress : QEvent::KeyRelease),
                                                   qtcode, modifiers, nativecode + 8, 0, int(modifiers),
                                                   (unicode != 0xffff) ? QString(QChar(unicode)) : QString(), autoRepeat);
}

EpaperEvdevKeyboardHandler::KeycodeAction EpaperEvdevKeyboardHandler::processKeycode(quint16 keycode, bool pressed, bool autorepeat)
{
    KeycodeAction result = None;
    bool first_press = pressed && !autorepeat;

    const EpaperEvdevKeyboardMap::Mapping *map_plain = 0;
    const EpaperEvdevKeyboardMap::Mapping *map_withmod = 0;

    quint8 modifiers = m_modifiers;

    // get a specific and plain mapping for the keycode and the current modifiers
    for (int i = 0; i < m_keymap_size && !(map_plain && map_withmod); ++i) {
        const EpaperEvdevKeyboardMap::Mapping *m = m_keymap + i;
        if (m->keycode == keycode) {
            if (m->modifiers == 0)
                map_plain = m;

            quint8 testmods = m_modifiers;
            if (m_locks[0] /*CapsLock*/ && (m->flags & EpaperEvdevKeyboardMap::IsLetter))
                testmods ^= EpaperEvdevKeyboardMap::ModShift;
            if (m->modifiers == testmods)
                map_withmod = m;
        }
    }

    if (m_locks[0] /*CapsLock*/ && map_withmod && (map_withmod->flags & EpaperEvdevKeyboardMap::IsLetter))
        modifiers ^= EpaperEvdevKeyboardMap::ModShift;

    qCDebug(EpaperEvdevKeyboardMapLog, "Processing key event: keycode=%3d, modifiers=%02x pressed=%d, autorepeat=%d  |  plain=%d, withmod=%d, size=%d",
            keycode, modifiers, pressed ? 1 : 0, autorepeat ? 1 : 0,
            int(map_plain ? map_plain - m_keymap : -1),
            int(map_withmod ? map_withmod - m_keymap : -1),
            m_keymap_size);

    const EpaperEvdevKeyboardMap::Mapping *it = map_withmod ? map_withmod : map_plain;

    if (!it) {
        // we couldn't even find a plain mapping
        qCDebug(EpaperEvdevKeyboardMapLog, "Could not find a suitable mapping for keycode: %3d, modifiers: %02x", keycode, modifiers);
        return result;
    }

    bool skip = false;
    quint16 unicode = it->unicode;
    quint32 qtcode = it->qtcode;

    if ((it->flags & EpaperEvdevKeyboardMap::IsModifier) && it->special) {
        // this is a modifier, i.e. Shift, Alt, ...
        if (pressed)
            m_modifiers |= quint8(it->special);
        else
            m_modifiers &= ~quint8(it->special);
    } else if (qtcode >= Qt::Key_CapsLock && qtcode <= Qt::Key_ScrollLock) {
        // (Caps|Num|Scroll)Lock
        if (first_press) {
            quint8 &lock = m_locks[qtcode - Qt::Key_CapsLock];
            lock ^= 1;

            switch (qtcode) {
            case Qt::Key_CapsLock:
                result = lock ? CapsLockOn : CapsLockOff;
                break;
            case Qt::Key_NumLock:
                result = lock ? NumLockOn : NumLockOff;
                break;
            case Qt::Key_ScrollLock:
                result = lock ? ScrollLockOn : ScrollLockOff;
                break;
            default:
                break;
            }
        }
    } else if ((it->flags & EpaperEvdevKeyboardMap::IsSystem) && it->special && first_press) {
        switch (it->special) {
        case EpaperEvdevKeyboardMap::SystemReboot:
            result = Reboot;
            break;

        case EpaperEvdevKeyboardMap::SystemZap:
            if (!m_no_zap)
                qApp->quit();
            break;

        case EpaperEvdevKeyboardMap::SystemConsolePrevious:
            result = PreviousConsole;
            break;

        case EpaperEvdevKeyboardMap::SystemConsoleNext:
            result = NextConsole;
            break;

        default:
            if (it->special >= EpaperEvdevKeyboardMap::SystemConsoleFirst && it->special <= EpaperEvdevKeyboardMap::SystemConsoleLast) {
                result = KeycodeAction(SwitchConsoleFirst + ((it->special & EpaperEvdevKeyboardMap::SystemConsoleMask) & SwitchConsoleMask));
            }
            break;
        }

        skip = true; // no need to tell Qt about it
    } else if ((qtcode == Qt::Key_Multi_key) && m_do_compose) {
        // the Compose key was pressed
        if (first_press)
            m_composing = 2;
        skip = true;
    } else if ((it->flags & EpaperEvdevKeyboardMap::IsDead) && m_do_compose) {
        // a Dead key was pressed
        if (first_press && m_composing == 1 && m_dead_unicode == unicode) { // twice
            m_composing = 0;
            qtcode = Qt::Key_unknown; // otherwise it would be Qt::Key_Dead...
        } else if (first_press && unicode != 0xffff) {
            m_dead_unicode = unicode;
            m_composing = 1;
            skip = true;
        } else {
            skip = true;
        }
    }

    if (!skip) {
        // a normal key was pressed
        const int modmask = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier | Qt::KeypadModifier;

        // we couldn't find a specific mapping for the current modifiers,
        // or that mapping didn't have special modifiers:
        // so just report the plain mapping with additional modifiers.
        if ((it == map_plain && it != map_withmod) || (map_withmod && !(map_withmod->qtcode & modmask))) {
            qtcode |= EpaperEvdevKeyboardHandler::toQtModifiers(modifiers);
        }

        if (m_composing == 2 && first_press && !(it->flags & EpaperEvdevKeyboardMap::IsModifier)) {
            // the last key press was the Compose key
            if (unicode != 0xffff) {
                int idx = 0;
                // check if this code is in the compose table at all
                for (; idx < m_keycompose_size; ++idx) {
                    if (m_keycompose[idx].first == unicode)
                        break;
                }
                if (idx < m_keycompose_size) {
                    // found it -> simulate a Dead key press
                    m_dead_unicode = unicode;
                    unicode = 0xffff;
                    m_composing = 1;
                    skip = true;
                } else {
                    m_composing = 0;
                }
            } else {
                m_composing = 0;
            }
        } else if (m_composing == 1 && first_press && !(it->flags & EpaperEvdevKeyboardMap::IsModifier)) {
            // the last key press was a Dead key
            bool valid = false;
            if (unicode != 0xffff) {
                int idx = 0;
                // check if this code is in the compose table at all
                for (; idx < m_keycompose_size; ++idx) {
                    if (m_keycompose[idx].first == m_dead_unicode && m_keycompose[idx].second == unicode)
                        break;
                }
                if (idx < m_keycompose_size) {
                    quint16 composed = m_keycompose[idx].result;
                    if (composed != 0xffff) {
                        unicode = composed;
                        qtcode = Qt::Key_unknown;
                        valid = true;
                    }
                }
            }
            if (!valid) {
                unicode = m_dead_unicode;
                qtcode = Qt::Key_unknown;
            }
            m_composing = 0;
        }

        if (!skip) {
            // Up until now qtcode contained both the key and modifiers. Split it.
            Qt::KeyboardModifiers qtmods = Qt::KeyboardModifiers(qtcode & modmask);
            qtcode &= ~modmask;

            // qtmods here is the modifier state before the event, i.e. not
            // including the current key in case it is a modifier.
            qCDebug(EpaperEvdevKeyboardMapLog, "Processing: uni=%04x, qt=%08x, qtmod=%08x", unicode, qtcode, int(qtmods));

            // If NumLockOff and keypad key pressed remap event sent
            if (!m_locks[1] && (qtmods & Qt::KeypadModifier) && keycode >= 71 && keycode <= 83 && keycode != 74 && keycode != 78) {

                unicode = 0xffff;
                switch (keycode) {
                case 71: // 7 --> Home
                    qtcode = Qt::Key_Home;
                    break;
                case 72: // 8 --> Up
                    qtcode = Qt::Key_Up;
                    break;
                case 73: // 9 --> PgUp
                    qtcode = Qt::Key_PageUp;
                    break;
                case 75: // 4 --> Left
                    qtcode = Qt::Key_Left;
                    break;
                case 76: // 5 --> Clear
                    qtcode = Qt::Key_Clear;
                    break;
                case 77: // 6 --> right
                    qtcode = Qt::Key_Right;
                    break;
                case 79: // 1 --> End
                    qtcode = Qt::Key_End;
                    break;
                case 80: // 2 --> Down
                    qtcode = Qt::Key_Down;
                    break;
                case 81: // 3 --> PgDn
                    qtcode = Qt::Key_PageDown;
                    break;
                case 82: // 0 --> Ins
                    qtcode = Qt::Key_Insert;
                    break;
                case 83: //, --> Del
                    qtcode = Qt::Key_Delete;
                    break;
                }
            }

            // Map SHIFT + Tab to SHIFT + Backtab, QShortcutMap knows about this translation
            if (qtcode == Qt::Key_Tab && (qtmods & Qt::ShiftModifier) == Qt::ShiftModifier)
                qtcode = Qt::Key_Backtab;

            // Generate the QPA event.
            processKeyEvent(keycode, unicode, qtcode, qtmods, pressed, autorepeat);
        }
    }
    return result;
}

// builtin keymaps
#include "map/epaperevdevkeyboardmap_no.h"
#include "map/epaperevdevkeyboardmap_us.h"
#include "map/epaperevdevkeyboardmap_us_rm.h"
#include "map/epaperevdevkeyboardmap_es.h"
#include "map/epaperevdevkeyboardmap_fr.h"
#include "map/epaperevdevkeyboardmap_uk.h"
#include "map/epaperevdevkeyboardmap_de.h"
#include "map/epaperevdevkeyboardmap_se.h"

void EpaperEvdevKeyboardHandler::unloadKeymap()
{
    qCDebug(EpaperEvdevKeyboardLog, "Unload current keymap and restore built-in");

    QSettings settings;
    QString locale = settings.value("InputLocale").toString();

    if (m_didLoadKeymap) {
        delete[] m_keymap;
        delete[] m_keycompose;
    }

    const EpaperEvdevKeyboardMap::Mapping *keymapFirst = nullptr;
    int keymapSize = 0;
    const EpaperEvdevKeyboardMap::Composing *keycomposeFirst = nullptr;
    int keycomposeSize = 0;

    // Check if input locale is set in xochitl.conf (set from type folio settings in the GUI).
    // If not, fetch the layout from the firmware and make assumptions (e.g. that nordic keyboard is Norwegian and not Swedish).
    auto keymap = EpaperEvdevInputLocale::UnitedStates;
    if (auto const keymapOpt = determineKeymapSettings())
    {
        qCDebug(EpaperEvdevKeyboardLog) << "Keymap has been determined by the QT settings.";
        keymap = keymapOpt.value();
    }
    else if (auto const keymapOpt = determineKeymapFirmware())
    {
        qCDebug(EpaperEvdevKeyboardLog) << "Keymap has been determined by the firmware settings.";
        keymap = keymapOpt.value();
    }
    else
    {
        qCWarning(EpaperEvdevKeyboardLog) << "No keymap set by QT settings or firmware, defaulting to US.";
    }
    m_prevLocale = keymap;

    switch (keymap) {
    case EpaperEvdevInputLocale::Denmark:
        // TODO: Change when Danish is implemented.
        keymapFirst = s_keymap_no;
        keymapSize = sizeof(s_keymap_no) / sizeof(s_keymap_no[0]);
        keycomposeFirst = s_keycompose_no;
        keycomposeSize = sizeof(s_keycompose_no) / sizeof(s_keycompose_no[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting Danish keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::Norway:
        keymapFirst = s_keymap_no;
        keymapSize = sizeof(s_keymap_no) / sizeof(s_keymap_no[0]);
        keycomposeFirst = s_keycompose_no;
        keycomposeSize = sizeof(s_keycompose_no) / sizeof(s_keycompose_no[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting Norwegian keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::Finland:
    case EpaperEvdevInputLocale::Sweden:
        // Finnish and Swedish layouts are the same.
        keymapFirst = s_keymap_se;
        keymapSize = sizeof(s_keymap_se) / sizeof(s_keymap_se[0]);
        keycomposeFirst = s_keycompose_se;
        keycomposeSize = sizeof(s_keycompose_se) / sizeof(s_keycompose_se[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting Swedish keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::UnitedKingdom:
        keymapFirst = s_keymap_uk;
        keymapSize = sizeof(s_keymap_uk) / sizeof(s_keymap_uk[0]);
        keycomposeFirst = s_keycompose_uk;
        keycomposeSize = sizeof(s_keycompose_uk) / sizeof(s_keycompose_uk[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting UK keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::UnitedStates:
        keymapFirst = s_keymap_us_rm;
        keymapSize = sizeof(s_keymap_us_rm) / sizeof(s_keymap_us_rm[0]);
        keycomposeFirst = s_keycompose_us_rm;
        keycomposeSize = sizeof(s_keycompose_us_rm) / sizeof(s_keycompose_us_rm[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting US keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::Spain:
        keymapFirst = s_keymap_es;
        keymapSize = sizeof(s_keymap_es) / sizeof(s_keymap_es[0]);
        keycomposeFirst = s_keycompose_es;
        keycomposeSize = sizeof(s_keycompose_es) / sizeof(s_keycompose_es[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting Spanish keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::France:
        keymapFirst = s_keymap_fr;
        keymapSize = sizeof(s_keymap_fr) / sizeof(s_keymap_fr[0]);
        keycomposeFirst = s_keycompose_fr;
        keycomposeSize = sizeof(s_keycompose_fr) / sizeof(s_keycompose_fr[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting French keymap" << keymapSize << keycomposeSize;
        break;
    case EpaperEvdevInputLocale::Germany:
        keymapFirst = s_keymap_de;
        keymapSize = sizeof(s_keymap_de) / sizeof(s_keymap_de[0]);
        keycomposeFirst = s_keycompose_de;
        keycomposeSize = sizeof(s_keycompose_de) / sizeof(s_keycompose_de[0]);
        qCDebug(EpaperEvdevKeyboardLog) << "setting German keymap" << keymapSize << keycomposeSize;
        break;
    default:
        qCWarning(EpaperEvdevKeyboardLog) << "setting *no* keymap! uh oh!";
    }

    EpaperEvdevKeyboardMap::Mapping *qmap_keymap = new EpaperEvdevKeyboardMap::Mapping[keymapSize];
    for (int i = 0; i < keymapSize; ++i) {
        qmap_keymap[i] = keymapFirst[i];
    }
    m_keymap = qmap_keymap;
    m_keymap_size = keymapSize;

    EpaperEvdevKeyboardMap::Composing *qmap_keycompose = new EpaperEvdevKeyboardMap::Composing[keycomposeSize];
    for (int i = 0; i < keycomposeSize; ++i) {
        qmap_keycompose[i] = keycomposeFirst[i];
    }
    m_keycompose = qmap_keycompose;
    m_keycompose_size = keycomposeSize;

    // we allocated the keymap, so we must unload it...
    m_didLoadKeymap = true;

    // reset state, so we could switch keymaps at runtime
    m_modifiers = 0;
    memset(m_locks, 0, sizeof(m_locks));
    m_composing = 0;
    m_dead_unicode = 0xffff;

    // Set locks according to keyboard leds
    quint16 ledbits[1];
    memset(ledbits, 0, sizeof(ledbits));
    if (::ioctl(m_fd.get(), EVIOCGLED(sizeof(ledbits)), ledbits) < 0) {
        qWarning("evdevkeyboard: Failed to query led states");
        switchLed(LED_NUML, false);
        switchLed(LED_CAPSL, false);
        switchLed(LED_SCROLLL, false);
    } else {
        // Capslock
        if ((ledbits[0] & 0x02) > 0)
            m_locks[0] = 1;
        // Numlock
        if ((ledbits[0] & 0x01) > 0)
            m_locks[1] = 1;
        // Scrollock
        if ((ledbits[0] & 0x04) > 0)
            m_locks[2] = 1;
        qCDebug(EpaperEvdevKeyboardLog, "numlock=%d , capslock=%d, scrolllock=%d", m_locks[1], m_locks[0], m_locks[2]);
    }
}

bool EpaperEvdevKeyboardHandler::loadKeymap(const QString &file)
{
    qCDebug(EpaperEvdevKeyboardLog, "Loading keymap %ls", qUtf16Printable(file));
    
    QFile f(file);

    if (!f.open(QIODevice::ReadOnly)) {
        qWarning("Could not open keymap file '%ls'", qUtf16Printable(file));
        return false;
    }

    // .qmap files have a very simple structure:
    // quint32 magic           (QKeyboard::FileMagic)
    // quint32 version         (1)
    // quint32 keymap_size     (# of struct QKeyboard::Mappings)
    // quint32 keycompose_size (# of struct QKeyboard::Composings)
    // all QKeyboard::Mappings via QDataStream::operator(<<|>>)
    // all QKeyboard::Composings via QDataStream::operator(<<|>>)

    quint32 qmap_magic, qmap_version, qmap_keymap_size, qmap_keycompose_size;

    QDataStream ds(&f);

    ds >> qmap_magic >> qmap_version >> qmap_keymap_size >> qmap_keycompose_size;

    if (ds.status() != QDataStream::Ok || qmap_magic != EpaperEvdevKeyboardMap::FileMagic || qmap_version != 1 || qmap_keymap_size == 0) {
        qWarning("'%ls' is not a valid .qmap keymap file", qUtf16Printable(file));
        return false;
    }

    EpaperEvdevKeyboardMap::Mapping *qmap_keymap = new EpaperEvdevKeyboardMap::Mapping[qmap_keymap_size];
    EpaperEvdevKeyboardMap::Composing *qmap_keycompose = qmap_keycompose_size ? new EpaperEvdevKeyboardMap::Composing[qmap_keycompose_size] : 0;

    for (quint32 i = 0; i < qmap_keymap_size; ++i)
        ds >> qmap_keymap[i];
    for (quint32 i = 0; i < qmap_keycompose_size; ++i)
        ds >> qmap_keycompose[i];

    if (ds.status() != QDataStream::Ok) {
        delete[] qmap_keymap;
        delete[] qmap_keycompose;

        qWarning("Keymap file '%ls' cannot be loaded.", qUtf16Printable(file));
        return false;
    }

    // unload currently active and clear state
    unloadKeymap();

    m_keymap = qmap_keymap;
    m_keymap_size = qmap_keymap_size;
    m_keycompose = qmap_keycompose;
    m_keycompose_size = qmap_keycompose_size;

    m_do_compose = true;
    m_didLoadKeymap = true;

    return true;
}

void EpaperEvdevKeyboardHandler::setCapsLockEnabled(bool enabled)
{
    m_locks[0] = enabled ? 1 : 0;
}

void EpaperEvdevKeyboardHandler::onSettingsChanged()
{
    // Need to remove and readd file on watchlist for some reason...
    m_watcher.removePath(qtSettings.fileName());
    m_watcher.addPath(qtSettings.fileName());

    // Skip if config change is irrelevant to us.
    if (auto const newLocale = determineKeymapSettings(); newLocale != m_prevLocale)
    {
        qCDebug(EpaperEvdevKeyboardLog) << "Input locale setting has changed, updating the key map.";
        unloadKeymap();
        // `m_prevLocale = newLocale` done inside unloadKeymap()
    }
}
QT_END_NAMESPACE
