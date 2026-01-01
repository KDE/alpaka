// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "ChatModel.h"

#include <KLocalizedString>
#include <KUser>

#include "kllmcoresettings.h"
#include <QJsonObject>
#include <QStringLiteral>
using namespace Qt::StringLiterals;
using namespace KLLMCore;
ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_llm{new KLLMInterface{this}}
{
    auto settings = KLLMCoreSettings::self();

    m_llm->setOllamaUrl(settings->serverUrl());

    QJsonObject systemMessage;
    systemMessage["role"_L1] = "system"_L1;
    systemMessage["content"_L1] = KLLMCoreSettings::systemPrompt();
    m_messageJsonHistory.append(systemMessage);

    connect(KLLMCoreSettings::self(), &KLLMCoreSettings::SystemPromptChanged, this, [this] {
        auto lastMessage = this->m_messageJsonHistory.last();
        if (lastMessage["role"_L1] == "system"_L1)
            this->m_messageJsonHistory.removeLast();
        QJsonObject systemMessage;
        systemMessage["role"_L1] = "system"_L1;
        systemMessage["content"_L1] = KLLMCoreSettings::systemPrompt();
        this->m_messageJsonHistory.append(systemMessage);
    });
    connect(KLLMCoreSettings::self(), &KLLMCoreSettings::ServerUrlChanged, this, [this] {
        m_llm->setOllamaUrl(KLLMCoreSettings::serverUrl());
    });

    auto setDefaultModelConnection = new QMetaObject::Connection;
    *setDefaultModelConnection = connect(m_llm, &KLLMInterface::readyChanged, this, [this, setDefaultModelConnection] {
        if (!m_llm->ready()) {
            return;
        }

        disconnect(*setDefaultModelConnection);
        delete setDefaultModelConnection;
        const auto models = m_llm->models();
        if (!models.contains(KLLMCoreSettings::model())) {
            KLLMCoreSettings::setModel(models.constFirst());
        }
    });
}

ChatModel::~ChatModel() = default;

QHash<int, QByteArray> ChatModel::roleNames() const
{
    return {{Roles::MessageRole, "message"},
            {Roles::SenderRole, "sender"},
            {Roles::FinishedRole, "finished"},
            {Roles::MessageIndexRole, "messageIndex"},
            {Roles::TokensPerSecondRole, "tokensPerSecond"},
            {Roles::TokenCountRole, "tokenCount"},
            {Roles::DurationRole, "duration"}};
}

int ChatModel::rowCount(const QModelIndex &) const
{
    return m_messages.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return {};

    const auto &message = m_messages[index.row()];

    switch (role) {
    case Roles::MessageIndexRole:
        return message.messageIndex;
    case Roles::MessageRole:
        return message.content;
    case Roles::SenderRole:
        return message.sender;
    case Roles::FinishedRole:
        return !message.inProgress;
    case Roles::TokensPerSecondRole: {
        if (message.inProgress)
            return 0;

        const double seconds = message.info.duration.count() / 1'000'000'000.0f;
        if (seconds == 0)
            return 0;

        return double(message.info.tokenCount) / seconds;
    }
    case Roles::TokenCountRole:
        return message.info.tokenCount;
    case Roles::DurationRole:
        return message.info.duration.count() / 1'000'000'000.0f;
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
    QJsonObject obj;
    obj["role"_L1] = "user"_L1;
    obj["content"_L1] = message;
    m_messageJsonHistory.append(obj);
    KLLMRequest req{m_messageJsonHistory};
    req.setModel(KLLMCoreSettings::model());

    auto rep = m_llm->getCompletion(req);

    beginInsertRows({}, m_messages.size(), m_messages.size() + 1);
    m_messages.append(ChatMessage{.content = message, .sender = Sender::User, .messageIndex = m_messages.size()});
    m_messages.append(ChatMessage{.inProgress = true, .sender = Sender::LLM, .messageIndex = m_messages.size(), .llmReply = rep});
    m_connections.insert(rep, connect(rep, &KLLMReply::contentAdded, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             message.content = message.llmReply->readResponse();
                             Q_EMIT dataChanged(index(i), index(i), {Roles::MessageRole});
                         }));
    m_connections.insert(rep, connect(rep, &KLLMReply::finished, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             m_connections.remove(message.llmReply);
                             if (!message.llmReply->isAborted()) {
                                 message.info = message.llmReply->info();
                                 m_messageJsonHistory.append(message.llmReply->getReplyJson()); // TODO: Summarize older chats befor appending
                             }
                             message.llmReply->deleteLater();
                             message.llmReply = nullptr;
                             message.inProgress = false;
                             Q_EMIT dataChanged(index(i),
                                                index(i),
                                                {Roles::FinishedRole, Roles::TokensPerSecondRole, Roles::TokenCountRole, Roles::DurationRole});
                             Q_EMIT replyInProgressChanged();
                         }));
    Q_EMIT replyInProgressChanged();
    endInsertRows();
}

void ChatModel::getModelInfo()
{
    KLLMRequest req{QJsonArray{}};
    req.setModel(KLLMCoreSettings::model());

    auto rep = m_llm->getModelInfo(req);
    beginInsertRows({}, m_messages.size(), m_messages.size() + 1);

    m_messages.append(ChatMessage{.content = i18n("Show model info."), .sender = Sender::User, .messageIndex = m_messages.size()});
    m_messages.append(ChatMessage{.inProgress = true, .sender = Sender::LLM, .messageIndex = m_messages.size(), .llmReply = rep});
    m_connections.insert(rep, connect(rep, &KLLMReply::contentAdded, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             message.content = message.llmReply->readResponse();
                             Q_EMIT dataChanged(index(i), index(i), {Roles::MessageRole});
                         }));
    m_connections.insert(rep, connect(rep, &KLLMReply::finished, this, [this, i = m_messages.size() - 1] {
                             auto &message = m_messages[i];
                             m_connections.remove(message.llmReply);
                             message.llmReply->deleteLater();
                             message.llmReply = nullptr;
                             message.inProgress = false;
                             Q_EMIT dataChanged(index(i),
                                                index(i),
                                                {Roles::FinishedRole, Roles::TokensPerSecondRole, Roles::TokenCountRole, Roles::DurationRole});
                             Q_EMIT replyInProgressChanged();
                         }));
    Q_EMIT replyInProgressChanged();
    endInsertRows();
}

void ChatModel::resetConversation()
{
    beginResetModel();
    for (const auto &connection : std::as_const(m_connections)) {
        disconnect(connection);
    }
    m_connections.clear();
    Q_EMIT replyInProgressChanged();
    m_messages.clear();
    m_messageJsonHistory = {};
    endResetModel();
}

void ChatModel::abortRes(qsizetype itemIndex)
{
    m_messages[itemIndex].llmReply->abort();
}

#include "moc_ChatModel.cpp"
