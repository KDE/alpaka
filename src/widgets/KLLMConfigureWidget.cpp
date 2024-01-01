// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMConfigureWidget.h"
#include "KLLMModelComboBox.h"
#include <KLocalizedString>
#include <QHBoxLayout>
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

    auto modelLayout = new QHBoxLayout;
    modelLayout->setObjectName(QStringLiteral("modelLayout"));
    mainLayout->addLayout(modelLayout);

    auto label = new QLabel(i18n("Models:"), this);
    label->setObjectName(QStringLiteral("label"));
    modelLayout->addWidget(label);

    mKLLMModelComboBox->setObjectName(QStringLiteral("mKLLMModelComboBox"));
    modelLayout->addWidget(mKLLMModelComboBox);
}

KLLMConfigureWidget::~KLLMConfigureWidget() = default;

int KLLMConfigureWidget::currentModel() const
{
    return mKLLMModelComboBox->currentIndex();
}

void KLLMConfigureWidget::setCurrentModel(int index)
{
    mKLLMModelComboBox->setCurrentIndex(index);
}

#include "moc_KLLMConfigureWidget.cpp"
