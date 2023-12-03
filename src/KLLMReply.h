#pragma once

#include <QNetworkReply>
#include <QJsonDocument>

class KLLMReply : public QObject
{
    Q_OBJECT

public:
    QString readResponse() const;

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

    int m_receivedSize = 0;
};
