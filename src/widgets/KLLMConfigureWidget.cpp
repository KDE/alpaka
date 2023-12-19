// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMConfigureWidget.h"
#include "KLLMModelComboBox.h"
#include <QLabel>
#include <QVBoxLayout>

using namespace KLLMWidgets;
KLLMConfigureWidget::KLLMConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mKLLMModelComboBox(new KLLMModelComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mKLLMModelComboBox->setObjectName(QStringLiteral("mKLLMModelComboBox"));
    mainLayout->addWidget(mKLLMModelComboBox);
}

KLLMConfigureWidget::~KLLMConfigureWidget() = default;

#include "moc_KLLMConfigureWidget.cpp"
