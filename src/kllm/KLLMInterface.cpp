// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMInterface.h"

#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace Qt::StringLiterals;

KLLMInterface::KLLMInterface(QObject *parent)
    : QObject{parent},
      m_manager{new QNetworkAccessManager{this}}
{
    // TODO: get this from ollama
    m_models.push_back(QStringLiteral("llama2"));

    QNetworkRequest req{QUrl::fromUserInput(QStringLiteral("http://0.0.0.0:11434/api/tags"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    auto rep = m_manager->get(req);
    connect(rep, &QNetworkReply::finished, this, [this, rep] {
        auto json = QJsonDocument::fromJson(rep->readAll());
        const auto models = json["models"_L1].toArray();
        for (const QJsonValue &model : models)
            m_models.push_back(model["name"_L1].toString());
        Q_EMIT modelsChanged();

        if (!m_models.isEmpty())
        {
            m_ready = true;
            Q_EMIT readyChanged();
        }
    });
}

bool KLLMInterface::ready() const
{
    return m_ready;
}

QStringList KLLMInterface::models() const
{
    return m_models;
}

KLLMReply *KLLMInterface::getCompletion(const KLLMRequest &request)
{
    Q_ASSERT(m_ready);

    QNetworkRequest req{QUrl::fromUserInput(QStringLiteral("http://0.0.0.0:11434/api/generate"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QJsonObject data;
    data["model"_L1] = request.model().isEmpty() ? m_models.first() : request.model();
    data["prompt"_L1] = request.message();
    data["context"_L1] = request.context().toJson();

    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));

    auto reply = new KLLMReply{m_manager->post(req, buf), this};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        Q_EMIT finished(reply->readResponse());
        buf->deleteLater();
    });
    return reply;
}
