// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMContext.h"

#include <QJsonValue>

using namespace KLLMCore;

QJsonValue KLLMContext::toJson() const
{
    switch (m_backend) {
    case Backend::Ollama:
        return m_data.value<QJsonArray>();
    default:
        return {};
    }
}

void KLLMContext::setOllamaContext(const QJsonArray &context)
{
    m_data = context;
    m_backend = Backend::Ollama;
}

QDebug operator<<(QDebug d, const KLLMContext &t)
{
    d << "m_data " << t.toJson();
    return d;
}
