// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "controller.h"
#include "kllmcoresettings.h"
#include "systemtray.h"
#include "windowcontroller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
#ifndef Q_OS_ANDROID
    setQuitOnLastWindowClosed();
    connect(KLLMCoreSettings::self(), &KLLMCoreSettings::SystemTrayChanged, this, &Controller::setQuitOnLastWindowClosed);
#endif
}

Controller::~Controller() = default;

Controller &Controller::instance()
{
    static Controller controller;
    return controller;
}

Controller *Controller::create(QQmlEngine *engine, QJSEngine *)
{
    engine->setObjectOwnership(&instance(), QQmlEngine::CppOwnership);
    return &instance();
}

void Controller::setQuitOnLastWindowClosed()
{
#ifndef Q_OS_ANDROID
    if (KLLMCoreSettings::self()->systemTray()) {
        mTrayIcon = new SystemTray(this);
        mTrayIcon->show();
        connect(mTrayIcon, &SystemTray::toggleWindow, this, &Controller::toggleWindow);
    } else {
        if (mTrayIcon) {
            disconnect(mTrayIcon, &SystemTray::toggleWindow, this, &Controller::toggleWindow);
            delete mTrayIcon;
            mTrayIcon = nullptr;
        }
    }
#endif
}

void Controller::toggleWindow()
{
    auto &instance = WindowController::instance();
    auto window = instance.window();
    if (window->isVisible()) {
        if (window->windowStates() & Qt::WindowMinimized) {
            window->showNormal();
            window->requestActivate();
        } else {
            window->close();
        }
    } else {
        instance.showAndRaiseWindow({});
        instance.window()->requestActivate();
    }
}

bool Controller::supportSystemTray() const
{
#ifdef Q_OS_ANDROID
    return false;
#else
    auto de = QString::fromLatin1(qgetenv("XDG_CURRENT_DESKTOP"));
    return de != QStringLiteral("GNOME") && de != QStringLiteral("Pantheon");
#endif
}

void Controller::saveWindowGeometry()
{
    WindowController::instance().saveGeometry();
}

#include "moc_controller.cpp"
