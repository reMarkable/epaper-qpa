/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "epaperintegration.h"
#include "epaperbackingstore.h"
#include "epaperevdevkeyboardmanager.h"

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/private/qpixmap_raster_p.h>
#include <qpa/qplatformfontdatabase.h>
#include <qpa/qplatforminputcontextfactory_p.h>
#include <qpa/qplatformwindow.h>

#include <private/qevdevmousemanager_p.h>
#include <private/qevdevtouchmanager_p.h>
#include <private/qgenericunixeventdispatcher_p.h>
#include <private/qgenericunixfontdatabase_p.h>

QT_BEGIN_NAMESPACE

EpaperIntegration::EpaperIntegration(const QStringList &parameters) :
    QPlatformIntegration(),
    QPlatformNativeInterface(),
    m_fontDatabase(0),
    m_inputContext(0)
{
    Q_UNUSED(parameters);
    EpaperScreen *mPrimaryScreen = new EpaperScreen();

    mPrimaryScreen->mGeometry = QRect(0, 0, 1404, 1872);

    mPrimaryScreen->mDepth = 32;
    mPrimaryScreen->mFormat = QImage::Format_RGB16;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
    QWindowSystemInterface::handleScreenAdded(mPrimaryScreen);
#else
    screenAdded(mPrimaryScreen);
#endif
}

EpaperIntegration::~EpaperIntegration()
{
    delete m_fontDatabase;
}

bool EpaperIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps:
        return true;
    case MultipleWindows:
        return true;
    default:
        return QPlatformIntegration::hasCapability(cap);
    }
}

void EpaperIntegration::initialize()
{
    m_keyboardManager = new EpaperEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), nullptr);
    new QEvdevTouchManager(QLatin1String("EvdevTouch"), QString() /* spec */, nullptr);

    m_inputContext = QPlatformInputContextFactory::create();
}

QPlatformFontDatabase *EpaperIntegration::fontDatabase() const
{
    if (!m_fontDatabase) {
        m_fontDatabase = new QGenericUnixFontDatabase();
    }
    return m_fontDatabase;
}

QPlatformInputContext *EpaperIntegration::inputContext() const
{
    return m_inputContext;
}

QPlatformWindow *EpaperIntegration::createPlatformWindow(QWindow *window) const
{
    QPlatformWindow *w = new QPlatformWindow(window);
    w->requestActivateWindow();
    return w;
}

QPlatformBackingStore *EpaperIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new EpaperBackingStore(window);
}

QAbstractEventDispatcher *EpaperIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QPlatformNativeInterface *EpaperIntegration::nativeInterface() const
{
    return const_cast<EpaperIntegration *>(this);
}

QFunctionPointer EpaperIntegration::platformFunction(const QByteArray &function) const
{
    // Don't change these strings. They must agree with xochitl.
    if (function == "rm_seabirdConnectionChanged") {
        return QFunctionPointer(seabirdConnectionChangedStatic);
    } else if (function == "rm_seabirdCapsLockEnable") {
        return QFunctionPointer(seabirdCapsLockEnableStatic);
    } else if (function == "rm_seabirdCapsLockDisable") {
        return QFunctionPointer(seabirdCapsLockDisableStatic);
    }

    return nullptr;
}

void EpaperIntegration::seabirdConnectionChangedStatic()
{
    EpaperIntegration *self = EpaperIntegration::instance();
    self->m_keyboardManager->loadKeymap(QString());
}

void EpaperIntegration::seabirdCapsLockEnableStatic()
{
    EpaperIntegration *self = EpaperIntegration::instance();
    self->m_keyboardManager->setCapsLockEnabled(true);
}

void EpaperIntegration::seabirdCapsLockDisableStatic()
{
    EpaperIntegration *self = EpaperIntegration::instance();
    self->m_keyboardManager->setCapsLockEnabled(false);
}

EpaperIntegration *EpaperIntegration::instance()
{
    return static_cast<EpaperIntegration *>(QGuiApplicationPrivate::platformIntegration());
}

QT_END_NAMESPACE
