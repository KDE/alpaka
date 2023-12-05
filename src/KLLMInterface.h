// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QNetworkAccessManager>

#include "KLLMRequest.h"
#include "KLLMReply.h"

class KLLMInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged FINAL)
    Q_PROPERTY(QStringList models READ models NOTIFY modelsChanged FINAL)

public:
    explicit KLLMInterface(QObject *parent = nullptr);

    bool ready() const;
    QStringList models() const;

public slots:
    KLLMReply *getCompletion(const KLLMRequest &equest);

signals:
    void finished(const QString &replyText);

    void readyChanged();
    void modelsChanged();

private:
    QNetworkAccessManager *m_manager;
    QStringList m_models;
    bool m_ready = false;
};
