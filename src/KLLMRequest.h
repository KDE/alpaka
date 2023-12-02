#pragma once

#include "KLLMContext.h"

class KLLMRequest : public QObject
{
    Q_OBJECT
public:
    explicit KLLMRequest(QString message, QObject *parent = nullptr);

    QString message() const;
    KLLMContext *context() const;

    void setMessage(const QString &message);
    void setContext(KLLMContext *context);

private:
    QString m_message;
    KLLMContext *m_context;
};
