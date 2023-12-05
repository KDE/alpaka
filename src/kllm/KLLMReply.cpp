// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMReply.h"

#include <QJsonArray>
#include <QJsonDocument>

using namespace Qt::StringLiterals;

QString KLLMReply::readResponse() const
{
    QString ret;
    for (const auto &tok : m_tokens)
        ret += tok["response"_L1].toString();
    return ret;
}

const KLLMContext &KLLMReply::context() const
{
    return m_context;
}

bool KLLMReply::isFinished() const
{
    return m_finished;
}

KLLMReply::KLLMReply(QNetworkReply *netReply, QObject *parent)
    : QObject{parent},
      m_reply{netReply}
{
    connect(m_reply, &QNetworkReply::finished, m_reply, [this] {
        // Normally, we could assume that the tokens will never be empty once the request finishes, but it could be possible
        // that the request failed and we have no tokens to parse.
        if (!m_tokens.empty())
            m_context.setOllamaContext(m_tokens.constLast()["context"_L1].toArray());

        qDebug() << "Ollama response finished";
        m_finished = true;
        Q_EMIT finished();
    });
    connect(m_reply, &QNetworkReply::errorOccurred, m_reply, [](QNetworkReply::NetworkError e) {
        qDebug() << "Ollama HTTP error:" << e;
    });
    connect(m_reply, &QNetworkReply::downloadProgress, m_reply, [this](qint64 received, qint64 /*total*/) {
        m_incompleteTokens += m_reply->read(received - m_receivedSize);
        m_receivedSize = received;

        auto completeTokens = m_incompleteTokens.split('\n');
        if (completeTokens.size() <= 1)
            return;
        m_incompleteTokens = completeTokens.last();
        completeTokens.removeLast();

        for (const auto &tok : completeTokens)
            m_tokens.append(QJsonDocument::fromJson(tok));

        Q_EMIT contentAdded();
    });
}
