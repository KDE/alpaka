// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// Code based on neochat code

#include "systemtray.h"
#include "windowcontroller.h"
#include <KWindowSystem>

SystemTray::SystemTray(QObject *parent)
    : KStatusNotifierItem{parent}
{
    setCategory(KStatusNotifierItem::ItemCategory::SystemServices);
    // FIXME: Fix icon !
    setIconByName(QStringLiteral("org.kde.kognos"));
    connect(this, &KStatusNotifierItem::activateRequested, this, [this] {
        KWindowSystem::setCurrentXdgActivationToken(providedToken());
        Q_EMIT toggleWindow();
    });

    connect(&WindowController::instance(), &WindowController::windowChanged, this, [this] {
        setAssociatedWindow(WindowController::instance().window());
    });
}

SystemTray::~SystemTray() = default;

void SystemTray::show()
{
    setStatus(Active);
}

void SystemTray::hide()
{
    setStatus(Passive);
}

#include "moc_systemtray.cpp"
