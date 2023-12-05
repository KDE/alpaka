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
    emit modelChanged();
}

void ChatModel::sendMessage(const QString &message)
{
    KLLMRequest req{message, this};
    req.setModel(m_model);
    for (int i = m_messages.size() - 1; i >= 0; --i)
    {
        if (m_messages[i].sender == Sender::LLM)
        {
            req.setContext(m_messages[i].context);
            break;
        }
    }
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
        message.context = message.llmReply->context();
        message.llmReply = nullptr;
    });
    endInsertRows();
}
