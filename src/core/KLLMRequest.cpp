// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMRequest.h"
#include <QJsonArray>

using namespace KLLMCore;
KLLMRequest::KLLMRequest(const QJsonArray &messages)
    : m_messages(messages)
{
}

QJsonArray KLLMRequest::messages() const
{
    return m_messages;
}

QString KLLMRequest::model() const
{
    return m_model;
}

void KLLMRequest::setMessages(const QJsonArray &messages)
{
    m_messages = messages;
}

void KLLMRequest::setModel(const QString &model)
{
    m_model = model;
}

QDebug operator<<(QDebug d, const KLLMRequest &t)
{
    d << "Model: " << t.model();
    d << "Message: " << t.messages();
    return d;
}
