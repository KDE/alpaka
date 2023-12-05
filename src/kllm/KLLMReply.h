// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "libkllmcore_export.h"
#include <QJsonDocument>
#include <QNetworkReply>

#include "KLLMContext.h"

/**
 * @brief The KLLMReply class represents a reply from an LLM.
 *
 * Requesting a completion from a KLLMInterface will return a KLLMReply. You can use this to track the progress of the LLM's
 * reply.
 *
 * If you want to stream a reply as it is written in real time, connect to contentAdded() and use readResponse() to retrieve
 * the new content. If you prefer to wait for the entire reply before displaying anything, connect to finished(), which will
 * only be emitted once the reply is complete.
 */
class LIBKLLMCORE_EXPORT KLLMReply : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Get the current response content.
     *
     * This function returns what it has recieved of the response so far. Therefore, until finished() is emitted, this
     * function may return different values. However, once finished() is emitted, the content is guaranteed to remain
     * constant.
     *
     * @return The content that has been returned so far.
     */
    [[nodiscard]] QString readResponse() const;

    /**
     * @brief Get the context token for this response.
     *
     * Messages sent by most LLMs have a context identifier that allows you to chain messages into a conversation. To create
     * such a conversation, you need to take this context object and set it on the next KLLMRequest in the conversation.
     * KLLMInterface::getCompletion() will use that context object to continue the message thread.
     *
     * @return A context object that refers to this response.
     */
    const KLLMContext &context() const;

    /**
     * @brief Check whether the reply has finished.
     *
     * If you need to know if the response has finished changing or if the context has been received yet, call this function.
     *
     * @return Whether the reply has finished.
     */
    bool isFinished() const;

protected:
    explicit KLLMReply(QNetworkReply *netReply, QObject *parent = nullptr);

    friend class KLLMInterface;

Q_SIGNALS:
    /**
     * @brief Emits when new content has been added to the response.
     *
     * If you are not streaming the response live, this signal is not of importance to you. However, if you are streaming
     * content, when this signal is emitted, you should call readResponse() to update the response that your application
     * shows.
     */
    void contentAdded();

    /**
     * @brief Emits when the LLM has finished returning its response.
     *
     * After this signal has emitted, the content is guaranteed to not change. At this point, you should call readResponse()
     * to get the content and then either take ownership of the KLLMReply or delete it, as automatic reply deletion is not
     * implemented yet.
     */
    void finished();

private:
    QNetworkReply *const m_reply;
    QByteArray m_incompleteTokens;

    QList<QJsonDocument> m_tokens;

    KLLMContext m_context;

    int m_receivedSize = 0;
    bool m_finished = false;
};
