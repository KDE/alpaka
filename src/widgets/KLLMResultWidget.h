// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once
#include "kllmwidgets_export.h"
#include <QWidget>
class QPlainTextEdit;
namespace KLLMWidgets
{
class KLLMWIDGETS_EXPORT KLLMResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KLLMResultWidget(QWidget *parent = nullptr);
    ~KLLMResultWidget() override;

    void setResult(const QString &str);

private:
    QPlainTextEdit *const mResult;
};
}
