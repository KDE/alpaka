// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// Code based on neochat code

#include "systemtray.h"
#include <KWindowSystem>

SystemTray::SystemTray(QObject *parent)
    : KStatusNotifierItem{parent}
{
    setCategory(KStatusNotifierItem::ItemCategory::ApplicationStatus);
    setIconByName(QStringLiteral("alpaka"));
    connect(this, &KStatusNotifierItem::activateRequested, this, [this] {
        KWindowSystem::setCurrentXdgActivationToken(providedToken());
        Q_EMIT toggleWindow();
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
