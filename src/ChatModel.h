#pragma once

#include <QAbstractListModel>

#include "KLLMInterface.h"
#include "KLLMReply.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        MessageRole,
        SenderRole,
    };

    enum Sender
    {
        LLM,
        User
    };
    Q_ENUM(Sender)

    ChatModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void sendMessage(const QString &message);

private:
    struct ChatMessage
    {
        bool inProgress = false;
        QString content;
        Sender sender;
        KLLMReply *llmReply;
    };

    QList<ChatMessage> m_messages;
    KLLMInterface *m_llm;
};
