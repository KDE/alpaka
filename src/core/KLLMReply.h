// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmcore_export.h"
#include <QJsonDocument>

#include "KLLMContext.h"

class QNetworkReply;
namespace KLLMCore
{
/**
 * @brief The KLLMReplyInfo class represents information about a reply from an LLM.
 *
 * When an LLM generates a completion, the server generally will return some information about the completion, including the
 * duration of the completion, the number of tokens received, and the duration of the prompt evaluation. This struct encapsulates such information.
 * If any one of these fields is not available, it will be set to its default value.
 */
struct KLLMCORE_EXPORT KLLMReplyInfo {
    //! The total time from when the request was received by the server to when the reply was returned.
    std::chrono::nanoseconds totalDuration;

    //! The time spent loading the model.
    std::chrono::nanoseconds loadDuration;

    //! The number of tokens in the prompt.
    int promptEvalTokenCount;

    //! The time spent evaluating the prompt.
    std::chrono::nanoseconds promptEvalDuration;

    //! The number of tokens in the reply.
    int tokenCount;

    //! The time spent generating the reply.
    std::chrono::nanoseconds duration;
};

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
class KLLMCORE_EXPORT KLLMReply : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Specifies the request type.
     *
     * When the class in instantiated the type of request should be specified
     */
    enum class RequestTypes {
        StreamingGenerate,
        Show
    };
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
     * @brief Get the context token for this response (if applicable).
     *
     * Messages sent by most LLMs have a context identifier that allows you to chain messages into a conversation. To create
     * such a conversation, you need to take this context object and set it on the next KLLMRequest in the conversation.
     * KLLMInterface::getCompletion() will use that context object to continue the message thread.
     *
     * @return A context object that refers to this response.
     */
    const KLLMContext &context() const;

    /**
     * @brief Get extra information about the reply (if applicable).
     *
     * This function returns a KLLMReplyInfo object containing information about this reply. If the reply has not finished, the KLLMReplyInfo object will have
     * all members set to their default values.
     *
     * @return Extra information about the reply.
     */
    const KLLMReplyInfo &info() const;

    /**
     * @brief Check whether the reply has finished.
     *
     * If you need to know if the response has finished changing or if the context has been received yet, call this function.
     *
     * @return Whether the reply has finished.
     */
    [[nodiscard]] bool isFinished() const;

    /**
     * @brief Get request type.
     *
     * The request type is set when this object is created.
     *
     * @return Corresponding request type.
     */
    const RequestTypes &requestType() const;

protected:
    explicit KLLMReply(QNetworkReply *netReply, QObject *parent = nullptr, RequestTypes requestType = RequestTypes::StreamingGenerate);

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
    KLLMReplyInfo m_info;
    RequestTypes m_requestType = RequestTypes::StreamingGenerate;

    int m_receivedSize = 0;
    bool m_finished = false;
};
}
