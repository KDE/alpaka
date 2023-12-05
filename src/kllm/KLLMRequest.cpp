// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMRequest.h"

KLLMRequest::KLLMRequest(const QString &message)
    : m_message{message}
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
