// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "ChatModel.h"

#include <KUser>

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_llm{new KLLMInterface{QStringLiteral("http://0.0.0.0:11434"), this}}
{
    KUser user;
    // Note (Loren): For some reason, Llama 2 kept trying to talk to me in asterisks text when it was prompted with my name no matter how I tweaked the system
    // prompt, so I just gave up and forbade it to use that mode of speech unless the user used it. Even so, it doesn't always behave.
    m_llm->setSystemPrompt(
        QStringLiteral("You are an AI assistant. You are speaking to a person named %1. Be helpful, professional, and courteous. Do not give inaccurate "
                       "information. Under no circumstances should you speak in asterisks text unless %1 speaks to you with asterisks text first.")
            .arg(user.property(KUser::UserProperty::FullName).toString()));
}

ChatModel::~ChatModel() = default;

QHash<int, QByteArray> ChatModel::roleNames() const
{
    return {{Roles::MessageRole, "message"}, {Roles::SenderRole, "sender"}};
}

int ChatModel::rowCount(const QModelIndex &) const
{
    return m_messages.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return {};

    switch (role) {
    case Roles::MessageRole:
        return m_messages[index.row()].content;
    case Roles::SenderRole:
        return m_messages[index.row()].sender;
    default:
        return {};
    }
}

KLLMInterface *ChatModel::llm() const
{
    return m_llm;
}

QString ChatModel::model() const
{
    return m_model;
}

void ChatModel::setModel(const QString &model)
{
    if (model == m_model)
        return;
    m_model = model;
    Q_EMIT modelChanged();
}

void ChatModel::sendMessage(const QString &message)
{
    KLLMRequest req{message};
    req.setModel(m_model);
    for (int i = m_messages.size() - 1; i >= 0; --i) {
        if (m_messages[i].sender == Sender::LLM) {
            req.setContext(m_messages[i].context);
            break;
        }
    }
    auto rep = m_llm->getCompletion(req);

    beginInsertRows({}, m_messages.size(), m_messages.size() + 1);
    m_messages.append(ChatMessage{.content = message, .sender = Sender::User});
    m_messages.append(ChatMessage{.sender = Sender::LLM, .llmReply = rep});
    m_connections.insert(rep, connect(rep, &KLLMReply::contentAdded, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             message.content = message.llmReply->readResponse();
                             Q_EMIT dataChanged(index(i), index(i), {Roles::MessageRole});
                         }));
    m_connections.insert(rep, connect(rep, &KLLMReply::finished, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             m_connections.remove(message.llmReply);
                             message.context = message.llmReply->context();
                             message.llmReply->deleteLater();
                             message.llmReply = nullptr;
                         }));
    endInsertRows();
}

void ChatModel::resetConversation()
{
    beginResetModel();
    for (const auto &connection : std::as_const(m_connections))
        disconnect(connection);
    m_messages.clear();
    endResetModel();
}