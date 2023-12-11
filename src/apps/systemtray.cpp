// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "systemtray.h"

SystemTray::SystemTray(QObject *parent)
    : KStatusNotifierItem{parent}
{
}

SystemTray::~SystemTray() = default;
