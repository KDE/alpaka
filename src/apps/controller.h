// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
class SystemTray;
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller() override;

    static Controller &instance();
    static Controller *create(QQmlEngine *engine, QJSEngine *);

private:
    void setQuitOnLastWindowClosed();
    void toggleWindow();
    SystemTray *mTrayIcon = nullptr;
};
