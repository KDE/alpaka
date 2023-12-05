#include "KLLMRequest.h"

KLLMRequest::KLLMRequest(QString message, QObject *parent)
    : QObject{parent},
      m_message{message}
{}

QString KLLMRequest::message() const
{
    return m_message;
}

const KLLMContext &KLLMRequest::context() const
{
    return m_context;
}

QString KLLMRequest::model() const
{
    return m_model;
}

void KLLMRequest::setMessage(const QString &message)
{
    m_message = message;
}

void KLLMRequest::setContext(const KLLMContext &context)
{
    m_context = context;
}

void KLLMRequest::setModel(const QString &model)
{
    m_model = model;
}
