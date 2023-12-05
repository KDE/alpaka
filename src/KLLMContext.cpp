// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMContext.h"

#include <QJsonValue>

QJsonValue KLLMContext::toJson() const
{
    switch (m_backend) {
    case Backend::Ollama:
        return m_data.value<QJsonArray>();
    default:
        return {};
    }
}

void KLLMContext::setOllamaContext(const QJsonArray &arr)
{
    m_data = arr;
    m_backend = Backend::Ollama;
}
