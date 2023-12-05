// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QNetworkAccessManager>

#include "KLLMRequest.h"
#include "KLLMReply.h"

/**
 * @brief The KLLMInterface class provides an interface around the LLM API.
 *
 * KLLM is designed to feel similar to Qt Network. This means that all LLM requests must be routed through a central
 * KLLMInterface object.
 *
 * To request a message or completion from the LLM, first create a KLLMRequest object with the desired message. Choose the
 * model from models() that you wish to use for this request and set it on the KLLMRequest. Then call getCompletion(),
 * passing your KLLMRequest object. You will recieve a KLLMReply object; connect to KLLMReply::contentAdded() if you wish to
 * receive content updates as they arrive from the server or connect to KLLMReply::finished() if you prefer to have the whole
 * message delivered at once.
 *
 * You should not request any completions (or otherwise use this class) until ready() returns true. Using the interface
 * before it is ready can cause problems; for example, the interface may not have loaded the available models yet or the
 * backend could be unreachable.
 */
class KLLMInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged FINAL)
    Q_PROPERTY(QStringList models READ models NOTIFY modelsChanged FINAL)

public:
    explicit KLLMInterface(QObject *parent = nullptr);

    /**
     * @brief Check whether the interface is ready.
     *
     * You should not use the interface until ready() returns true. Failure to observe this rule may result in undefined
     * behavior.
     *
     * @return Returns whether the interface is ready.
     */
    bool ready() const;

    /**
     * @brief Retrieve a list of models supported by the LLM backend.
     *
     * When creating a KLLMRequest, you should choose a model from this list for the request. If you do not specify a model,
     * the request will probably fail.
     *
     * @return Returns a QStringList containing all valid models for this interface.
     */
    QStringList models() const;

public slots:
    /**
     * @brief Request a completion from the LLM.
     *
     * Calling this function starts a request to the LLM backend. You should use the returned KLLMReply pointer to track the
     * status of the LLM's response. Once the KLLMReply emits KLLMReply::finished(), it is your responsibility to either
     * track or delete the KLLMReply; auto-deleting is not implemented yet.
     *
     * @param request The request object that will be used to create the actual LLM request.
     * @return Returns a pointer to a KLLMReply that can be used to track the progress of the reply.
     */
    KLLMReply *getCompletion(const KLLMRequest &request);

signals:
    /**
     * @brief This signal is emitted when any completion requested by the interface is completed.
     * @param replyText Contains the text of the completion.
     */
    void finished(const QString &replyText);

    void readyChanged();
    void modelsChanged();

private:
    QNetworkAccessManager *m_manager;
    QStringList m_models;
    bool m_ready = false;
};
