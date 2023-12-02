#pragma once

#include <QNetworkAccessManager>

#include "KLLMRequest.h"
#include "KLLMReply.h"

class KLLMInterface : public QObject
{
    Q_OBJECT
public:
    explicit KLLMInterface(QObject *parent = nullptr);

public slots:
    KLLMReply *getCompletion(const KLLMRequest &equest);
    KLLMReply *getCompletion(const QString &prompt);

signals:
    void finished(const QString &replyText);

private:
    QNetworkAccessManager *m_manager;
    QStringList m_models;
};
