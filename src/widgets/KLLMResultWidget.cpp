// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMResultWidget.h"
#include <QPlainTextEdit>
#include <QVBoxLayout>

using namespace KLLMWidgets;
KLLMResultWidget::KLLMResultWidget(QWidget *parent)
    : QWidget{parent}
    , mResult(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mResult->setObjectName(QStringLiteral("mResult"));
    mainLayout->addWidget(mResult);
    mResult->setReadOnly(true);
}

KLLMResultWidget::~KLLMResultWidget() = default;

void KLLMResultWidget::setResult(const QString &str)
{
    mResult->setPlainText(str);
}

#include "moc_KLLMResultWidget.cpp"
