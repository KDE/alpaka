#include "ChatModel.h"

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel{parent},
      m_llm{new KLLMInterface{this}}
{}

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

    switch (role)
    {
    case Roles::MessageRole:
        return m_messages[index.row()].content;
    case Roles::SenderRole:
        return m_messages[index.row()].sender;
    default:
        return {};
    }
}

void ChatModel::sendMessage(const QString &message)
{
    KLLMRequest req{message, this};
    auto rep = m_llm->getCompletion(req);

    beginInsertRows({}, m_messages.size(), m_messages.size() + 1);
    m_messages.append(ChatMessage{.content = message, .sender = Sender::User});
    m_messages.append(ChatMessage{.sender = Sender::LLM, .llmReply = rep});
    connect(rep, &KLLMReply::contentAdded, this, [this, i = m_messages.size() - 1] {
        auto &message = m_messages[i];
        message.content = message.llmReply->readResponse();
        emit dataChanged(index(i), index(i), {Roles::MessageRole});
    });
    connect(rep, &KLLMReply::finished, this, [this, i = m_messages.size() - 1] {
        auto &message = m_messages[i];
        message.llmReply = nullptr;
    });
    endInsertRows();
}
