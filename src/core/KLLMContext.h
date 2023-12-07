// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once
#include "kllmcore_export.h"
#include <QDebug>
#include <QJsonArray>

/**
 * @brief KLLMContext provides a representation of a conversation context.
 *
 * Most, if not all, LLMs have the concept of "context". This allows them to refer to previous messages in a conversation to
 * enhance their replies. In most scenarios, this is the preferred behavior.
 *
 * To use KLLMContext, you simply need to get the context from each KLLMReply and set it on the next KLLMReqeust that you
 * send. KLLMInterface will use this in KLLMInterface::getCompletion().
 */
struct KLLMCORE_EXPORT KLLMContext {
    /**
     * @brief Converts the context to a JSON representation.
     *
     * Different LLM backends represent context in different ways; for example, while Ollama represents context as an array
     * of integer identifiers, OpenAI relies on a JSON array of all the messages in the conversation so far. Therefore, this
     * function exists to take any representation set on it for any backend and convert it to a JSON value suitable for
     * sending in a request.
     *
     * @return A JSON representation of the context.
     */
    [[nodiscard]] QJsonValue toJson() const;

    /**
     * @brief Sets an Ollama context as the current context.
     * @param context The context from Ollama.
     */
    void setOllamaContext(const QJsonArray &context);

private:
    enum class Backend {
        Ollama,
    } m_backend;
    QVariant m_data;
};
KLLMCORE_EXPORT QDebug operator<<(QDebug d, const KLLMContext &t);
