// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QJsonDocument>
#include <QNetworkReply>

#include "KLLMContext.h"

class KLLMReply : public QObject
{
    Q_OBJECT

public:
    QString readResponse() const;
    const KLLMContext &context() const;

protected:
    explicit KLLMReply(QNetworkReply *netReply, QObject *parent = nullptr);

    friend class KLLMInterface;

signals:
    void contentAdded();
    void finished();

private:
    QNetworkReply *const m_reply;
    QByteArray m_incompleteTokens;

    QList<QJsonDocument> m_tokens;

    KLLMContext m_context;

    int m_receivedSize = 0;
};
