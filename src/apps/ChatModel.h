// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QAbstractListModel>
#include <QJsonArray>
#include <QtQmlIntegration>

#include "KLLMContext.h"
#include "KLLMInterface.h"
#include "KLLMReply.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

    QML_ELEMENT

    Q_PROPERTY(KLLMCore::KLLMInterface *llm READ llm CONSTANT FINAL)
    Q_PROPERTY(bool replyInProgress READ replyInProgress NOTIFY replyInProgressChanged FINAL)

public:
    enum Roles {
        // Basic roles
        MessageRole,
        SenderRole,
        FinishedRole,
        MessageIndexRole,

        // Debug information roles
        TokensPerSecondRole,
        TokenCountRole,
        DurationRole,
    };

    enum Sender {
        LLM,
        User,
    };
    Q_ENUM(Sender)

    explicit ChatModel(QObject *parent = nullptr);
    ~ChatModel() override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] int rowCount(const QModelIndex & = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] KLLMCore::KLLMInterface *llm() const;
    [[nodiscard]] bool replyInProgress() const;

public Q_SLOTS:
    void sendMessage(const QString &message);
    void getModelInfo();
    void resetConversation();
    void abortRes(qsizetype itemIndex);

Q_SIGNALS:
    void replyInProgressChanged();

private:
    struct ChatMessage {
        bool inProgress = false;
        QString content;
        Sender sender;

        qsizetype messageIndex = -1;
        KLLMCore::KLLMReply *llmReply = nullptr;
        KLLMCore::KLLMReplyInfo info;
    };

    QList<ChatMessage> m_messages;
    QJsonArray m_messageJsonHistory;
    KLLMCore::KLLMInterface *const m_llm;
    KLLMCore::KLLMContext *const m_context;
    QMultiHash<KLLMCore::KLLMReply *, QMetaObject::Connection> m_connections;
    QTimer *m_systemPromptTimer;
};
