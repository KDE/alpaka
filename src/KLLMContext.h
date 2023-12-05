// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QJsonArray>

struct KLLMContext
{
    QJsonValue toJson() const;

    void setOllamaContext(const QJsonArray &arr);

private:
    enum class Backend
    {
        Ollama,
    } m_backend;
    QVariant m_data;
};
