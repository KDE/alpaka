// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmcore_export.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

namespace KLLMCore
{
/**
 * @brief The KLLMMessage class keeps data of each message.
 *
 * Model options like 'thinking' should be placed in this class
 */
class KLLMCORE_EXPORT KLLMMessage
{
public:
    QString role;
    QString content;
    QVector<QByteArray> images;

    KLLMMessage() = default;

    KLLMMessage(const QString &role, const QString &content, const QVector<QByteArray> &images = {});
    QJsonObject toJson() const;
};
/**
 * @brief The KLLMContent class provides a container for history of messages sent and received.
 *
 * This class provides methods to add messages with different roles so it can transfer it between modules.
 *
 * Eventually its content will be converted to the appropriate format to be sent to a target model by the interface layer.
 *
 * Methods are self explaining
 */
class KLLMCORE_EXPORT KLLMContext : public QObject
{
public:
    KLLMContext(QObject *parent);

    void addUserMessage(const QString &content);

    void addAssistantMessage(const QString &content);

    void addSystemMessage(const QString &content);

    [[nodiscard]] const QVector<KLLMMessage> &messages() const;

    void clear();

    [[nodiscard]] int size() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] QJsonArray toJsonArray() const;

    [[nodiscard]] QJsonDocument toJsonDocument() const;

private:
    QVector<KLLMMessage> m_messages;
    QString parseMentions(const QString &message, QVector<QByteArray> &imagesOut);
    void handleFileUrl(const QUrl &url, QString &replacement, QString &error, QVector<QByteArray> &imagesOut);
};
}