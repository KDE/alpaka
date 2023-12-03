#include "KLLMInterface.h"

#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

KLLMInterface::KLLMInterface(QObject *parent)
    : QObject{parent},
      m_manager{new QNetworkAccessManager{this}}
{
    // TODO: get this from ollama
    m_models.push_back("llama2");

    QNetworkRequest req{{"http://0.0.0.0:11434/api/tags"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    auto rep = m_manager->get(req);
    connect(rep, &QNetworkReply::finished, this, [this, rep] {
        auto json = QJsonDocument::fromJson(rep->readAll());
        for (const QJsonValue &model : json["models"].toArray())
            m_models.push_back(model["name"].toString());
        emit modelsChanged();

        if (m_models.size() > 0)
        {
            m_ready = true;
            emit readyChanged();
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

    QNetworkRequest req{{"http://0.0.0.0:11434/api/generate"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data["model"] = request.model().isEmpty() ? m_models.first() : request.model();
    data["prompt"] = request.message();

    auto buf = new QBuffer{this};
    buf->setData(QJsonDocument(data).toJson(QJsonDocument::Compact));

    auto reply = new KLLMReply{m_manager->post(req, buf), this};
    connect(reply, &KLLMReply::finished, this, [this, reply, buf] {
        emit finished(reply->readResponse());
        buf->deleteLater();
    });
    return reply;
}

KLLMReply *KLLMInterface::getCompletion(const QString &prompt)
{
    // TODO: choose a default model once support for choosing models is implemented
    return getCompletion(KLLMRequest(prompt));
}
