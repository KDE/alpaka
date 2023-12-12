// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QAbstractListModel>
#include <QtQmlIntegration>

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
        MessageRole,
        SenderRole,
        FinishedRole,
    };

    enum Sender {
        LLM,
        User,
    };
    Q_ENUM(Sender)

    explicit ChatModel(QObject *parent = nullptr);
    ~ChatModel() override;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] KLLMCore::KLLMInterface *llm() const;
    [[nodiscard]] bool replyInProgress() const;

public Q_SLOTS:
    void sendMessage(const QString &message);
    void resetConversation();

Q_SIGNALS:
    void replyInProgressChanged();

private:
    struct ChatMessage {
        bool inProgress = false;
        QString content;
        Sender sender;
        KLLMCore::KLLMReply *llmReply;
        KLLMCore::KLLMContext context;
    };

    QList<ChatMessage> m_messages;
    KLLMCore::KLLMInterface *const m_llm;

    QMultiHash<KLLMCore::KLLMReply *, QMetaObject::Connection> m_connections;
};
