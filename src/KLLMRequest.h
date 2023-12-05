#pragma once

#include <QObject>

#include "KLLMContext.h"

class KLLMRequest : public QObject
{
    Q_OBJECT

public:
    explicit KLLMRequest(QString message, QObject *parent = nullptr);

    QString message() const;
    const KLLMContext &context() const;
    QString model() const;

    void setMessage(const QString &message);
    void setContext(const KLLMContext &context);
    void setModel(const QString &model);

private:
    QString m_message;
    KLLMContext m_context;
    QString m_model;
};
