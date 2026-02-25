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
using namespace KLLMCore;

KLLMInterface::KLLMInterface(QObject *parent)
    : KLLMInterface{QString{}, parent}
{
}

KLLMInterface::KLLMInterface(const QString &ollamaUrl, QObject *parent)
    : QObject{parent}
    , m_manager{new QNetworkAccessManager{this}}
    , m_ollamaUrl{ollamaUrl}
{
    if (!m_ollamaUrl.isEmpty())
        reload();
}

KLLMInterface::KLLMInterface(const QUrl &ollamaUrl, QObject *parent)
    : KLLMInterface{ollamaUrl.toString(), parent}
{
}

bool KLLMInterface::ready() const
{
    return m_ready && !m_hasError;
}

bool KLLMInterface::hasError() const
{
    return m_hasError;
}

QStringList KLLMInterface::models() const
{
    return m_models;
}

#if 0
void KLLMInterface::deleteModel(const QString &modelName)
{
    Q_ASSERT(ready());

    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/delete"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    QJsonObject data;
    data["name"_L1] = modelName;

    // Delete resource doesn't take argument. Need to look at how to do it.
    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));

    auto reply = new KLLMReply{m_manager->deleteResource(req, buf), this};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        Q_EMIT finished(reply->readResponse());
        buf->deleteLater();
    });
}
#endif

KLLMReply *KLLMInterface::getCompletion(const KLLMRequest &request)
{
    Q_ASSERT(ready());

    qDebug() << "Outgoing request:" << request;
    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/chat"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    QJsonObject data;
    data["model"_L1] = request.model().isEmpty() ? m_models.constFirst() : request.model();
    QJsonArray messagesJson;
    for (auto t : request.context()->messages()) {
        messagesJson.append(t.toJson());
    }
    data["messages"_L1] = messagesJson;

    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));
    auto reply = new KLLMReply{m_manager->post(req, buf), this};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        Q_EMIT finished(reply->readResponse());
        buf->deleteLater();
    });
    return reply;
}

KLLMReply *KLLMInterface::getModelInfo(const KLLMRequest &request)
{
    Q_ASSERT(ready());

    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/show"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QJsonObject data;
    data["model"_L1] = request.model().isEmpty() ? m_models.constFirst() : request.model();

    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));

    auto reply = new KLLMReply{m_manager->post(req, buf), this, KLLMReply::RequestTypes::Show};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        Q_EMIT finished(reply->readResponse());
        buf->deleteLater();
    });
    return reply;
}

void KLLMInterface::reload()
{
    if (m_ollamaCheck)
        disconnect(m_ollamaCheck);

    QNetworkRequest req{QUrl::fromUserInput(m_ollamaUrl + QStringLiteral("/api/tags"))};
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    auto rep = m_manager->get(req);
    m_ollamaCheck = connect(rep, &QNetworkReply::finished, this, [this, rep] {
        if (rep->error() != QNetworkReply::NoError) {
            Q_EMIT errorOccurred(i18nc("%1 is an URL, %2 an error description", "Failed to connect to interface at %1: %2", m_ollamaUrl, rep->errorString()));
            m_hasError = true;
            Q_EMIT readyChanged();
            Q_EMIT hasErrorChanged();
            return;
        }

        const auto json = QJsonDocument::fromJson(rep->readAll());
        const auto models = json["models"_L1].toArray();
        for (const QJsonValue &model : models) {
            m_models.push_back(model["name"_L1].toString());
        }
        Q_EMIT modelsChanged();

        m_ready = !m_models.isEmpty();
        m_hasError = false;
        Q_EMIT readyChanged();
        Q_EMIT hasErrorChanged();
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
    reload();
}

void KLLMInterface::setOllamaUrl(const QUrl &ollamaUrl)
{
    setOllamaUrl(ollamaUrl.toString());
}

#include "moc_KLLMInterface.cpp"
