// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMRequest.h"
#include <QJsonArray>
#include <QStringBuilder>

using namespace KLLMCore;
KLLMRequest::KLLMRequest(KLLMContext *context)
    : m_context(context)
{
}

KLLMContext *KLLMRequest::context() const
{
    return m_context;
}

QString KLLMRequest::model() const
{
    return m_model;
}

void KLLMRequest::setModel(const QString &model)
{
    m_model = model;
}

QDebug operator<<(QDebug d, const KLLMRequest &t)
{
    d << "Model: " << t.model();
    d << "Message: " << t.context()->toJsonDocument();
    return d;
}
