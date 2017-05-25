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

#include "qminimalintegration.h"
#include "qminimalbackingstore.h"

#include <QtGui/private/qpixmap_raster_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformfontdatabase.h>
//#include <QtFontDatabaseSupport/private/qgenericunixfontdatabase_p.h>
#include <private/qevdevkeyboardmanager_p.h>
#include <private/qevdevtouchmanager_p.h>
#include <private/qevdevmousemanager_p.h>

//#include <QtInputSupport/private/qevdevkeyboardmanager_p.h>
//#include <QtInputSupport/private/qevdevtouchmanager_p.h>
//#include <QtInputSupport/private/qevdevmousemanager_p.h>

#include <private/qgenericunixfontdatabase_p.h>
//#include <QtEventDispatcherSupport/private/qgenericunixeventdispatcher_p.h>
#include <private/qgenericunixeventdispatcher_p.h>
//#include <QtPlatformSupport/private/qtslib_p.h>

QT_BEGIN_NAMESPACE

static const char debugBackingStoreEnvironmentVariable[] = "QT_DEBUG_BACKINGSTORE";

static inline unsigned parseOptions(const QStringList &paramList)
{
    unsigned options = 0;
    for (const QString &param : paramList) {
        if (param == QLatin1String("enable_fonts"))
            options |= QMinimalIntegration::EnableFonts;
    }
    return options;
}

QMinimalIntegration::QMinimalIntegration(const QStringList &parameters) : QObject(),
    m_fontDatabase(0)
    , m_options(parseOptions(parameters))
{
    if (qEnvironmentVariableIsSet(debugBackingStoreEnvironmentVariable)
        && qEnvironmentVariableIntValue(debugBackingStoreEnvironmentVariable) > 0) {
        m_options |= DebugBackingStore | EnableFonts;
    }
    qDebug() << "EPD platform plugin loaded!";

    QMinimalScreen *mPrimaryScreen = new QMinimalScreen();

    mPrimaryScreen->mGeometry = QRect(0, 0, 1404, 1872);
    //mPrimaryScreen->mGeometry = QRect(0, 0, 1872, 1404);

    mPrimaryScreen->mDepth = 32;
    mPrimaryScreen->mFormat = QImage::Format_RGB16;

    screenAdded(mPrimaryScreen);
}

QMinimalIntegration::~QMinimalIntegration()
{
    qDebug() << "Minimal integration dying";
    delete m_fontDatabase;
    qDebug() << "Minimal integration dead";
}

bool QMinimalIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    case MultipleWindows: return true;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

void QMinimalIntegration::initialize()
{
    new QEvdevKeyboardManager(QLatin1String("EvdevKeyboard"), QString(), nullptr);
    new QEvdevTouchManager(QLatin1String("EvdevTouch"), QString() /* spec */, nullptr);
    //new QEvdevMouseManager(QLatin1String("EvdevMouse"), QString() /* spec */, qApp);
    //new QTsLibMouseHandler(QLatin1String("TsLib"), QString());
}

// Dummy font database that does not scan the fonts directory to be
// used for command line tools like qmlplugindump that do not create windows
// unless DebugBackingStore is activated.
class DummyFontDatabase : public QPlatformFontDatabase
{
public:
    virtual void populateFontDatabase() Q_DECL_OVERRIDE {}
};

QPlatformFontDatabase *QMinimalIntegration::fontDatabase() const
{
    if (!m_fontDatabase) {
        //m_fontDatabase = new QFontconfigDatabase();
        m_fontDatabase = new QGenericUnixFontDatabase();
    }
    return m_fontDatabase;
}

QPlatformWindow *QMinimalIntegration::createPlatformWindow(QWindow *window) const
{
    Q_UNUSED(window);
    qDebug() << "Creating window";
    QPlatformWindow *w = new QPlatformWindow(window);
    w->requestActivateWindow();
    return w;
}

QPlatformBackingStore *QMinimalIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QMinimalBackingStore(window);
}

QAbstractEventDispatcher *QMinimalIntegration::createEventDispatcher() const
{
    return createUnixEventDispatcher();
}

QMinimalIntegration *QMinimalIntegration::instance()
{
    return static_cast<QMinimalIntegration *>(QGuiApplicationPrivate::platformIntegration());
}

QT_END_NAMESPACE
