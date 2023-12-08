// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "ChatModel.h"

#include <KUser>

#include "kandalf.h"
using namespace KLLMCore;
ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_llm{new KLLMInterface{this}}
{
    auto settings = KandalfSettings::self();

    m_llm->setOllamaUrl(settings->serverUrl());
    m_llm->setSystemPrompt(settings->systemPrompt());

    connect(KandalfSettings::self(), &KandalfSettings::SystemPromptChanged, this, [this] {
        m_llm->setSystemPrompt(KandalfSettings::systemPrompt());
    });

    auto setDefaultModelConnection = new QMetaObject::Connection;
    *setDefaultModelConnection = connect(m_llm, &KLLMInterface::readyChanged, this, [this, setDefaultModelConnection] {
        if (!m_llm->ready())
            return;

        disconnect(*setDefaultModelConnection);
        delete setDefaultModelConnection;
        const auto models = m_llm->models();
        if (!models.contains(KandalfSettings::model()))
            KandalfSettings::setModel(models.first());
    });
}

ChatModel::~ChatModel() = default;

QHash<int, QByteArray> ChatModel::roleNames() const
{
    return {{Roles::MessageRole, "message"}, {Roles::SenderRole, "sender"}, {Roles::FinishedRole, "finished"}};
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
    case Roles::FinishedRole:
        return !m_messages[index.row()].inProgress;
    default:
        return {};
    }
}

KLLMInterface *ChatModel::llm() const
{
    return m_llm;
}

bool ChatModel::replyInProgress() const
{
    // If any replies are still connected to their slots, that means they haven't finished yet.
    return !m_connections.empty();
}

void ChatModel::sendMessage(const QString &message)
{
    KLLMRequest req{message};
    req.setModel(KandalfSettings::model());
    for (int i = m_messages.size() - 1; i >= 0; --i) {
        if (m_messages[i].sender == Sender::LLM) {
            req.setContext(m_messages[i].context);
            break;
        }
    }
    auto rep = m_llm->getCompletion(req);

    beginInsertRows({}, m_messages.size(), m_messages.size() + 1);
    m_messages.append(ChatMessage{.content = message, .sender = Sender::User});
    m_messages.append(ChatMessage{.inProgress = true, .sender = Sender::LLM, .llmReply = rep});
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
                             message.inProgress = false;
                             Q_EMIT dataChanged(index(i), index(i), {Roles::FinishedRole});
                             Q_EMIT replyInProgressChanged();
                         }));
    Q_EMIT replyInProgressChanged();
    endInsertRows();
}

void ChatModel::resetConversation()
{
    beginResetModel();
    for (const auto &connection : std::as_const(m_connections))
        disconnect(connection);
    m_connections.clear();
    Q_EMIT replyInProgressChanged();
    m_messages.clear();
    endResetModel();
}
