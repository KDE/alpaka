// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmcore_export.h"

#include <QDebug>
#include <QJsonArray>
namespace KLLMCore
{
/**
 * @brief The KLLMRequest class contains a representation of a message to an LLM.
 *
 * Before calling KLLMInterface::getCompletion(), you need to create a KLLMRequest with information about the desired
 * completion.
 */
class KLLMCORE_EXPORT KLLMRequest
{
public:
    /**
     * @brief Creates a KLLMRequest with the array of messages \a messages.
     * @param messages The json array of messages that will be sent to the LLM.
     * @param parent
     */
    explicit KLLMRequest(const QJsonArray &messages);

    /**
     * @brief Use this to get the array of messages for the LLM.
     * @return Returns the array of messages to be sent to the LLM.
     */
    [[nodiscard]] QJsonArray messages() const;

    /**
     * @brief Gets the LLM model that will be used by the backend.
     * @return The name of the model to be used for this request.
     */
    [[nodiscard]] QString model() const;

    /**
     * @brief Sets the messages to be sent to the LLM.
     * @param messages The array of messages in json format to be sent to the LLM.
     */
    void setMessages(const QJsonArray &messages);

    /**
     * @brief Sets the model to be used for this request.
     *
     * You should set this to one of the values returned by KLLMInterface::models(); failure to do so will likely produce an
     * error from the backend.
     *
     * @param model The name of the model to be used for this request.
     */
    void setModel(const QString &model);

private:
    QJsonArray m_messages;
    QString m_model;
};
}
KLLMCORE_EXPORT QDebug operator<<(QDebug d, const KLLMCore::KLLMRequest &t);
