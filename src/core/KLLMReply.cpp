// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMReply.h"
#include "kllmcore_debug.h"

#include <QNetworkReply>

using namespace Qt::StringLiterals;
using namespace KLLMCore;

KLLMReply::KLLMReply(QNetworkReply *netReply, QObject *parent, RequestTypes requestType)
    : QObject{parent}
    , m_reply{netReply}
    , m_requestType{requestType}
{
    connect(m_reply, &QNetworkReply::finished, m_reply, [this] {
        // Normally, we could assume that the tokens will never be empty once the request finishes, but it could be possible
        // that the request failed and we have no tokens to parse.
        if (m_requestType == RequestTypes::StreamingGenerate && !m_tokens.empty()) {
            const auto finalResponse = m_tokens.constLast();
            m_context.setOllamaContext(finalResponse["context"_L1].toArray());
            m_info.totalDuration = std::chrono::nanoseconds{finalResponse["total_duration"_L1].toVariant().toULongLong()};
            m_info.loadDuration = std::chrono::nanoseconds{finalResponse["load_duration"_L1].toVariant().toULongLong()};
            m_info.promptEvalTokenCount = finalResponse["prompt_eval_count"_L1].toVariant().toULongLong();
            m_info.promptEvalDuration = std::chrono::nanoseconds{finalResponse["prompt_eval_duration"_L1].toVariant().toULongLong()};
            m_info.tokenCount = finalResponse["eval_count"_L1].toVariant().toULongLong();
            m_info.duration = std::chrono::nanoseconds{finalResponse["eval_duration"_L1].toVariant().toULongLong()};
        }

        qCDebug(KLLMCORE_LOG) << "Ollama response finished";
        m_finished = true;
        Q_EMIT finished();
    });
    connect(m_reply, &QNetworkReply::errorOccurred, m_reply, [](QNetworkReply::NetworkError e) {
        qCDebug(KLLMCORE_LOG) << "Ollama HTTP error:" << e;
    });
    connect(m_reply, &QNetworkReply::downloadProgress, m_reply, [this](qint64 received, qint64 /*total*/) {
        m_incompleteTokens += m_reply->read(received - m_receivedSize);
        m_receivedSize = received;
        switch (m_requestType) {
        case RequestTypes::Show:
            m_tokens.append(QJsonDocument::fromJson(m_incompleteTokens));
            break;
        case RequestTypes::StreamingGenerate:
            auto completeTokens = m_incompleteTokens.split('\n');
            if (completeTokens.size() <= 1) {
                return;
            }
            m_incompleteTokens = completeTokens.last();
            completeTokens.removeLast();

            m_tokens.reserve(completeTokens.count());
            for (const auto &tok : std::as_const(completeTokens)) {
                m_tokens.append(QJsonDocument::fromJson(tok));
            }
            break;
        }
        Q_EMIT contentAdded();
    });
}

QString KLLMReply::readResponse() const
{
    QString ret;
    switch (m_requestType) {
    case RequestTypes::Show:
        ret += QString::fromLatin1("## Template: \n```\n") + m_tokens.constFirst()["template"_L1].toString() + QString::fromLatin1("\n```\n");
        ret += QString::fromLatin1("## Modelfile: \n```\n") + m_tokens.constFirst()["modelfile"_L1].toString() + QString::fromLatin1("\n```\n");
        ret += QString::fromLatin1("## Parameters: \n```\n") + m_tokens.constFirst()["parameters"_L1].toString() + QString::fromLatin1("\n```\n");
        ret += QString::fromLatin1("## Details: \n```\n")
            + QString::fromLatin1(QJsonDocument::fromVariant(m_tokens.constFirst()["details"_L1].toVariant()).toJson()) + QString::fromLatin1("\n```\n");
        ret += QString::fromLatin1("## Model Info: \n```\n")
            + QString::fromLatin1(QJsonDocument::fromVariant(m_tokens.constFirst()["model_info"_L1].toVariant()).toJson()) + QString::fromLatin1("\n```\n");
        break;
    case RequestTypes::StreamingGenerate:
        for (const auto &tok : m_tokens)
            ret += tok["response"_L1].toString();
        break;
    }

    return ret;
}

const KLLMContext &KLLMReply::context() const
{
    return m_context;
}

const KLLMReplyInfo &KLLMReply::info() const
{
    return m_info;
}

const KLLMReply::RequestTypes &KLLMReply::requestType() const
{
    return m_requestType;
}

bool KLLMReply::isFinished() const
{
    return m_finished;
}
#include "moc_KLLMReply.cpp"
