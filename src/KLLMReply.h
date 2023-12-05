#pragma once

#include <QNetworkReply>
#include <QJsonDocument>

#include "KLLMContext.h"

class KLLMReply : public QObject
{
    Q_OBJECT

public:
    QString readResponse() const;
    const KLLMContext &context() const;

protected:
    explicit KLLMReply(QNetworkReply *netReply, QObject *parent = nullptr);

    friend class KLLMInterface;

signals:
    void contentAdded();
    void finished();

private:
    QNetworkReply *m_reply;
    QByteArray m_incompleteTokens;

    QList<QJsonDocument> m_tokens;

    KLLMContext m_context;

    int m_receivedSize = 0;
};
