#include "KLLMInterface.h"

#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

KLLMInterface::KLLMInterface(QObject *parent)
    : QObject{parent},
      m_manager{new QNetworkAccessManager{this}}
{
    // TODO: get this from ollama
    m_models.push_back("llama2");
}

KLLMReply *KLLMInterface::getCompletion(const KLLMRequest &request)
{
    QNetworkRequest req{{"http://0.0.0.0:11434/api/generate"}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data["model"] = "llama2";
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
