// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "libkllmcore_export.h"
#include <QObject>

#include "KLLMContext.h"

class LIBKLLMCORE_EXPORT KLLMRequest : public QObject
{
    Q_OBJECT

public:
    explicit KLLMRequest(const QString &message, QObject *parent = nullptr);

    [[nodiscard]] QString message() const;
    const KLLMContext &context() const;
    [[nodiscard]] QString model() const;

    void setMessage(const QString &message);
    void setContext(const KLLMContext &context);
    void setModel(const QString &model);

private:
    QString m_message;
    KLLMContext m_context;
    QString m_model;
};
