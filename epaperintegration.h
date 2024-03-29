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

#ifndef QPLATFORMINTEGRATION_EPAPER_H
#define QPLATFORMINTEGRATION_EPAPER_H

#include <qpa/qplatformintegration.h>
#include <qpa/qplatformnativeinterface.h>
#include <qpa/qplatformscreen.h>

#include <QPointer>

QT_BEGIN_NAMESPACE

class EpaperScreen : public QPlatformScreen
{
public:
    EpaperScreen() :
        mDepth(32), mFormat(QImage::Format_ARGB32_Premultiplied) { }

    QRect geometry() const override { return mGeometry; }
    int depth() const override { return mDepth; }
    QImage::Format format() const override { return mFormat; }
    QSizeF physicalSize() const override
    {
        static const int dpi = 228;
        return QSizeF(geometry().size()) / dpi * qreal(25.4);
    }

public:
    QRect mGeometry;
    int mDepth;
    QImage::Format mFormat;
    QSize mPhysicalSize;
};

class EpaperEvdevKeyboardManager;

class EpaperIntegration : public QPlatformIntegration, public QPlatformNativeInterface
{
public:
    explicit EpaperIntegration(const QStringList &parameters);
    ~EpaperIntegration();

    bool hasCapability(QPlatformIntegration::Capability cap) const override;
    QPlatformFontDatabase *fontDatabase() const override;
    QPlatformInputContext *inputContext() const override;

    void initialize() override;

    QPlatformWindow *createPlatformWindow(QWindow *window) const override;
    QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const override;
    QAbstractEventDispatcher *createEventDispatcher() const override;
    QPlatformNativeInterface *nativeInterface() const override;
    QFunctionPointer platformFunction(const QByteArray &function) const override;
    void setCapsLockEnabled(bool enabled);

    static EpaperIntegration *instance();

private:
    static void seabirdConnectionChangedStatic();
    static void seabirdCapsLockEnableStatic();
    static void seabirdCapsLockDisableStatic();
    static void seabirdSetInputFlavorWindows();
    static void seabirdSetInputFlavorApple();

    mutable QPlatformFontDatabase *m_fontDatabase;
    QPlatformInputContext *m_inputContext;
    QPointer<EpaperEvdevKeyboardManager> m_keyboardManager;
};

QT_END_NAMESPACE

#endif
