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

#include "epaperevdevkeyboardmanager.h"

#include <QCoreApplication>
#include <QLoggingCategory>
#include <QString>
#include <QStringView>

#include <private/qguiapplication_p.h>
#include <private/qinputdevicemanager_p_p.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcEvdevKey)

// This code is originally from QEvdevUtil, extracted to keep this code independent.
namespace EpaperEvdevUtil {

struct ParsedSpecification
{
    QString spec;
    QList<QString> devices;
    QList<QStringView> args;
};

ParsedSpecification parseSpecification(const QString &specification)
{
    ParsedSpecification result;

    result.args = QStringView{specification}.split(QLatin1Char(':'));

    for (const QStringView &arg : qAsConst(result.args)) {
        if (arg.startsWith(QString("/dev/"))) {
            // if device is specified try to use it
            result.devices.append(arg.toString());
        } else {
            // build new specification without /dev/ elements
            result.spec += arg.toString() + QLatin1Char(':');
        }
    }

    if (!result.spec.isEmpty())
        result.spec.chop(1); // remove trailing ':'

    return result;
}

}

EpaperEvdevKeyboardManager::EpaperEvdevKeyboardManager(const QString &key, const QString &specification, QObject *parent) :
    QObject(parent)
{
    Q_UNUSED(key);

    QString spec = QString::fromLocal8Bit(qgetenv("QT_QPA_EVDEV_KEYBOARD_PARAMETERS"));

    if (spec.isEmpty())
        spec = specification;

    auto parsed = EpaperEvdevUtil::parseSpecification(spec);
    m_spec = std::move(parsed.spec);

    // add all keyboards for devices specified in the argument list
    for (const QString &device : qAsConst(parsed.devices))
        addKeyboard(device);

    if (parsed.devices.isEmpty()) {
        qCDebug(qLcEvdevKey, "evdevkeyboard: Using device discovery");
        if (auto deviceDiscovery = QDeviceDiscovery::create(QDeviceDiscovery::Device_Keyboard, this)) {
            // scan and add already connected keyboards
            const QStringList devices = deviceDiscovery->scanConnectedDevices();
            for (const QString &device : devices)
                addKeyboard(device);

            connect(deviceDiscovery, &QDeviceDiscovery::deviceDetected,
                    this, &EpaperEvdevKeyboardManager::addKeyboard);
            connect(deviceDiscovery, &QDeviceDiscovery::deviceRemoved,
                    this, &EpaperEvdevKeyboardManager::removeKeyboard);
        }
    }
}

EpaperEvdevKeyboardManager::~EpaperEvdevKeyboardManager()
{
}

void EpaperEvdevKeyboardManager::addKeyboard(const QString &deviceNode)
{
    qCDebug(qLcEvdevKey, "Adding keyboard at %ls", qUtf16Printable(deviceNode));
    auto keyboard = EpaperEvdevKeyboardHandler::create(deviceNode, m_spec);
    if (keyboard) {
        m_keyboards.add(deviceNode, std::move(keyboard));
        updateDeviceCount();
    } else {
        qWarning("Failed to open keyboard device %ls", qUtf16Printable(deviceNode));
    }
}

void EpaperEvdevKeyboardManager::removeKeyboard(const QString &deviceNode)
{
    if (m_keyboards.remove(deviceNode)) {
        qCDebug(qLcEvdevKey, "Removing keyboard at %ls", qUtf16Printable(deviceNode));
        updateDeviceCount();
    }
}

void EpaperEvdevKeyboardManager::updateDeviceCount()
{
    QInputDeviceManagerPrivate::get(QGuiApplicationPrivate::inputDeviceManager())->setDeviceCount(QInputDeviceManager::DeviceTypeKeyboard, m_keyboards.count());
}

void EpaperEvdevKeyboardManager::setCapsLockEnabled(bool enabled)
{
    for (const auto &keyboard : m_keyboards) {
        keyboard.handler->setCapsLockEnabled(enabled);
    }
}

void EpaperEvdevKeyboardManager::setInputFlavor(EpaperEvdevKeyboardMap::InputFlavor flavor)
{
    for (auto const &keyboard : m_keyboards) {
        keyboard.handler->setInputFlavor(flavor);
    }
}

void EpaperEvdevKeyboardManager::resetKeymap()
{
    for (const auto &keyboard : m_keyboards) {
        keyboard.handler->resetKeymap();
    }
}

QT_END_NAMESPACE
