// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMInterface.h"

#include <KLocalizedString>

#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace Qt::StringLiterals;

KLLMInterface::KLLMInterface(QObject *parent)
    : KLLMInterface{QString{}, parent}
{
}

KLLMInterface::KLLMInterface(const QString &ollamaUrl, QObject *parent)
    : QObject{parent}
    , m_manager{new QNetworkAccessManager{this}}
    , m_ollamaUrl{ollamaUrl}
{
    checkIfInterfaceIsValid();
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

    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/generate"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QJsonObject data;
    data["model"_L1] = request.model().isEmpty() ? m_models.constFirst() : request.model();
    data["prompt"_L1] = request.message();

    const auto context = request.context().toJson();
    if (!context.isNull())
        data["context"_L1] = context;

    if (!m_systemPrompt.isEmpty())
        data["system"_L1] = m_systemPrompt;

    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));

    auto reply = new KLLMReply{m_manager->post(req, buf), this};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        Q_EMIT finished(reply->readResponse());
        buf->deleteLater();
    });
    return reply;
}

void KLLMInterface::checkIfInterfaceIsValid()
{
    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/tags"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    auto rep = m_manager->get(req);
    connect(rep, &QNetworkReply::finished, this, [this, rep] {
        if (rep->error() != QNetworkReply::NoError) {
            Q_EMIT errorOccurred(i18n("Failed to connect to interface at %1: %2", m_ollamaUrl, rep->errorString()));
            m_ready = false;
            Q_EMIT readyChanged();
            return;
        }

        auto json = QJsonDocument::fromJson(rep->readAll());
        const auto models = json["models"_L1].toArray();
        for (const QJsonValue &model : models)
            m_models.push_back(model["name"_L1].toString());
        Q_EMIT modelsChanged();

        m_ready = !m_models.isEmpty();
        Q_EMIT readyChanged();
    });
}

QString KLLMInterface::ollamaUrl() const
{
    return m_ollamaUrl;
}

void KLLMInterface::setOllamaUrl(const QString &ollamaUrl)
{
    if (m_ollamaUrl == ollamaUrl)
        return;
    m_ollamaUrl = ollamaUrl;
    Q_EMIT ollamaUrlChanged();
    checkIfInterfaceIsValid();
}

void KLLMInterface::setOllamaUrl(const QUrl &ollamaUrl)
{
    setOllamaUrl(ollamaUrl.toString());
}

QString KLLMInterface::systemPrompt() const
{
    return m_systemPrompt;
}

void KLLMInterface::setSystemPrompt(const QString &systemPrompt)
{
    if (m_systemPrompt == systemPrompt)
        return;
    m_systemPrompt = systemPrompt;
    Q_EMIT systemPromptChanged();
}
