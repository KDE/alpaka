// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// Code based on neochat code

#include "windowcontroller.h"

#include "config-alpaka.h"
#include <KConfig>
#include <KWindowConfig>

#ifdef HAVE_WINDOWSYSTEM
#if HAVE_X11
#include <KStartupInfo>
#endif
#include <KWindowSystem>
#endif

#include <KSharedConfig>

WindowController &WindowController::instance()
{
    static WindowController instance;
    return instance;
}

WindowController *WindowController::create(QQmlEngine *engine, QJSEngine *)
{
    engine->setObjectOwnership(&instance(), QQmlEngine::CppOwnership);
    return &instance();
}

void WindowController::setWindow(QWindow *window)
{
    mWindow = window;

    Q_EMIT windowChanged();
}

QWindow *WindowController::window() const
{
    return mWindow;
}

void WindowController::restoreGeometry()
{
    const auto stateConfig = KSharedConfig::openStateConfig();
    const KConfigGroup windowGroup = stateConfig->group(QStringLiteral("Window"));

    KWindowConfig::restoreWindowSize(mWindow, windowGroup);
    KWindowConfig::restoreWindowPosition(mWindow, windowGroup);
}

void WindowController::saveGeometry()
{
    const auto stateConfig = KSharedConfig::openStateConfig();
    KConfigGroup windowGroup = stateConfig->group(QStringLiteral("Window"));

    KWindowConfig::saveWindowPosition(mWindow, windowGroup);
    KWindowConfig::saveWindowSize(mWindow, windowGroup);

    stateConfig->sync();
}

void WindowController::showAndRaiseWindow(const QString &startupId)
{
    if (!mWindow->isVisible()) {
        mWindow->show();
        restoreGeometry();
    }

#ifdef HAVE_WINDOWSYSTEM
    if (!startupId.isEmpty()) {
        if (KWindowSystem::isPlatformX11()) {
#if HAVE_X11
            KStartupInfo::setNewStartupId(mWindow, startupId.toUtf8());
#endif
        } else if (KWindowSystem::isPlatformWayland()) {
            KWindowSystem::setCurrentXdgActivationToken(startupId);
        }
    }

    KWindowSystem::activateWindow(mWindow);
#endif
}

#include "moc_windowcontroller.cpp"
