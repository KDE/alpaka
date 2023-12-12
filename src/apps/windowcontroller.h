// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// Code based on neochat code

#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#ifdef HAVE_WINDOWSYSTEM
#include <KWindowEffects>
#endif
/**
 * @class WindowController
 *
 * A singleton class to help manage the NeoChat window.
 */
class WindowController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static WindowController &instance();
    static WindowController *create(QQmlEngine *engine, QJSEngine *);

    /**
     * @brief Set the window that the will be managed.
     */
    void setWindow(QWindow *window);

    /**
     * @brief Get the window that the will be managed.
     */
    [[nodiscard]] QWindow *window() const;

    /**
     * @brief Restore any saved window geometry if available.
     */
    void restoreGeometry();

    /**
     * @brief Save the current window geometry.
     */
    void saveGeometry();

    /**
     * @brief Show the window and raise to the top.
     */
    void showAndRaiseWindow(const QString &startupId);

Q_SIGNALS:
    /**
     * @brief Triggered if the managed window is changed.
     */
    void windowChanged();

private:
    WindowController() = default;

    QWindow *mWindow = nullptr;
};
