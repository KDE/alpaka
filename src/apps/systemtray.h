// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// Code based on neochat code

#pragma once

#include <KStatusNotifierItem>

class SystemTray : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = nullptr);
    ~SystemTray() override;

    /**
     * @brief Show the tray icon.
     */
    void show();

    /**
     * @brief Hide the tray icon.
     */
    void hide();

Q_SIGNALS:
    /**
     * @brief Triggered when the system tray icon is clicked to request Kandalf be shown or hidden.
     */
    void toggleWindow();
};
