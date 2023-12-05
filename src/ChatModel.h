// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QAbstractListModel>

#include "KLLMInterface.h"
#include "KLLMReply.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(KLLMInterface *llm READ llm CONSTANT FINAL)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged FINAL)

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

    explicit ChatModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    KLLMInterface *llm() const;
    QString model() const;

    void setModel(const QString &model);

public slots:
    void sendMessage(const QString &message);

signals:
    void modelChanged();

private:
    struct ChatMessage
    {
        bool inProgress = false;
        QString content;
        Sender sender;
        KLLMReply *llmReply;
        KLLMContext context;
    };

    QList<ChatMessage> m_messages;
    KLLMInterface *const m_llm;
    QString m_model;
};
