#include "KLLMReply.h"

#include <QJsonDocument>

QString KLLMReply::readResponse() const
{
    QString ret;
    for (const auto &tok : m_tokens)
        ret += tok["response"].toString();
    return ret;
}

KLLMReply::KLLMReply(QNetworkReply *netReply, QObject *parent)
    : QObject{parent},
      m_reply{netReply}
{
    connect(m_reply, &QNetworkReply::finished, m_reply, [this] {
        qDebug() << "Ollama response finished";
        emit finished();
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
    });
}
