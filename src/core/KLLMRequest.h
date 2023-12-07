// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmcore_export.h"

#include "KLLMContext.h"
#include <QDebug>

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
     * @brief Creates a KLLMRequest with the message \a message.
     * @param message The message that will be sent to the LLM.
     * @param parent
     */
    explicit KLLMRequest(const QString &message);

    /**
     * @brief Use this to get the message for the LLM.
     * @return Returns the message to be sent to the LLM.
     */
    [[nodiscard]] QString message() const;

    /**
     * @brief Use this to retrive the context for the LLM.
     * @return Returns the context object to be sent to the LLM.
     */
    const KLLMContext &context() const;

    /**
     * @brief Gets the LLM model that will be used by the backend.
     * @return The name of the model to be used for this request.
     */
    [[nodiscard]] QString model() const;

    /**
     * @brief Sets the message to be sent to the LLM.
     * @param message The message text to be sent to the LLM.
     */
    void setMessage(const QString &message);

    /**
     * @brief Sets the conversation context for the LLM.
     *
     * If you want the LLM to respond in context of previous messages, you should set this to the context returned in the
     * previous KLLMReply.
     *
     * @param context The context object for this request.
     */
    void setContext(const KLLMContext &context);

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
    QString m_message;
    KLLMContext m_context;
    QString m_model;
};
KLLMCORE_EXPORT QDebug operator<<(QDebug d, const KLLMRequest &t);
