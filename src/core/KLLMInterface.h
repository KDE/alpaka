// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmcore_export.h"

#include "KLLMReply.h"
#include "KLLMRequest.h"
class QNetworkAccessManager;
namespace KLLMCore
{
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
class KLLMCORE_EXPORT KLLMInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged FINAL)
    Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged FINAL)
    Q_PROPERTY(QStringList models READ models NOTIFY modelsChanged FINAL)
    Q_PROPERTY(QString ollamaUrl READ ollamaUrl WRITE setOllamaUrl NOTIFY ollamaUrlChanged FINAL)

public:
    /**
     * @brief Creates a KLLMInterface.
     * @param parent The parent QObject.
     */
    explicit KLLMInterface(QObject *parent = nullptr);

    /**
     * @brief Creates a KLLMInterface with the url set to \a ollamaUrl.
     * @param ollamaUrl The URL to the Ollama instance.
     * @param parent The parent QObject.
     */
    explicit KLLMInterface(const QString &ollamaUrl, QObject *parent = nullptr);

    /**
     * @brief Creates a KLLMInterface with the url set to \a ollamaUrl.
     * @param ollamaUrl The URL to the Ollama instance.
     * @param parent The parent QObject.
     */
    explicit KLLMInterface(const QUrl &ollamaUrl, QObject *parent = nullptr);

    /**
     * @brief Check whether the interface is ready.
     *
     * You should not use the interface until ready() returns true. Failure to observe this rule may result in undefined behavior.
     *
     * If the interface encounters an error, ready() will return false. However, do not use ready() to indicate to the user that the interface is in an error
     * state, as the interface could be in the process of making its initial connection. Instead, you should use hasError() to check for an error state.
     * Additionally, you should connect to errorOccurred() to handle errors as they arise.
     *
     * @return Returns whether the interface is ready.
     */
    [[nodiscard]] bool ready() const;

    /**
     * @brief Check whether the interface is in an error state.
     *
     * After you handle an error from errorEmitted(), you should monitor this property. When it becomes \c false, you can safely resume operations.
     *
     * @return Returns whether the interface is in an error state.
     */
    [[nodiscard]] bool hasError() const;

    /**
     * @brief Retrieve a list of models supported by the LLM backend.
     *
     * When creating a KLLMRequest, you should choose a model from this list for the request. If you do not specify a model,
     * the request will probably fail.
     *
     * @return Returns a QStringList containing all valid models for this interface.
     */
    [[nodiscard]] QStringList models() const;

    /**
     * @brief Get the URL to the Ollama instance.
     * @return The URL for the Ollama instance.
     */
    [[nodiscard]] QString ollamaUrl() const;

    /**
     * @brief Set the URL to the Ollama instance.
     *
     * Since Ollama is a self-hostable service, users may wish to use different instances. Use this function to set the URL to the desired instance. It should
     * \a not contain the \c /api portion of the URL.
     *
     * @param ollamaUrl The new URL for the Ollama instance.
     */
    void setOllamaUrl(const QString &ollamaUrl);

    /**
     * @brief A convenience overload of setOllamaUrl() that takes a QUrl.
     * @param ollamaUrl The new URL for the Ollama instance.
     */
    void setOllamaUrl(const QUrl &ollamaUrl);

public Q_SLOTS:
    /**
     * @brief Request a completion from the LLM.
     *
     * Calling this function starts a request to the LLM backend. You should use the returned KLLMReply pointer to track the
     * status of the LLM's response. Once the KLLMReply emits KLLMReply::finished(), it is your responsibility to either
     * track or delete the KLLMReply; auto-deleting is not implemented yet.// Parameters
     *
     * Ollama API for /chat:
     *     model: (required) the model name
     *     messages: the messages of the chat, this can be used to keep a chat memory
     *     tools: list of tools in JSON for the model to use if supported
     *     think: (for thinking models) should the model think before responding?
     *
     *     The message object has the following fields:
     *       role: the role of the message, either system, user, assistant, or tool
     *       content: the content of the message
     *       thinking: (for thinking models) the model's thinking process
     *       images (optional): a list of images to include in the message (for multimodal models such as llava)
     *       tool_calls (optional): a list of tools in JSON that the model wants to use
     *       tool_name (optional): add the name of the tool that was executed to inform the model of the result
     *
     * @param request The request object that will be used to create the actual LLM request.
     * @return Returns a pointer to a KLLMReply that can be used to track the progress of the reply.
     */
    KLLMReply *getCompletion(const KLLMRequest &request);

    /**
     * @brief Request model info from Ollama.
     *
     * Calling this function starts a request to the LLM backend. You should use the returned KLLMReply pointer to track the
     * status of the LLM's response. Once the KLLMReply emits KLLMReply::finished(), it is your responsibility to either
     * track or delete the KLLMReply; auto-deleting is not implemented yet.
     *
     * @param request The request object that will be used to create the actual LLM request.
     * @return Returns a pointer to a KLLMReply that can be used to track the progress of the reply.
     */
    KLLMReply *getModelInfo(const KLLMRequest &request);

    /**
     * @brief Reload the LLM interface.
     *
     * Reloading the interface can be used to check if a network error is gone or to see if the available models have changed.
     */
    void reload();

Q_SIGNALS:
    /**
     * @brief This signal is emitted when any completion requested by the interface is completed.
     * @param replyText Contains the text of the completion.
     */
    void finished(const QString &replyText);

    void readyChanged();
    void hasErrorChanged();
    void modelsChanged();
    void ollamaUrlChanged();

    /**
     * @brief An error occurred while communicating with the interface.
     * @param message Contains the human readable error message.
     */
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager *const m_manager;
    QStringList m_models;
    bool m_ready = false;
    bool m_hasError = false;
    QString m_ollamaUrl;
    QMetaObject::Connection m_ollamaCheck;
};
}
