// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMConfigureDialog.h"
#include "KLLMConfigureWidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace KLLMWidgets;

KLLMConfigureDialog::KLLMConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mKLLMConfigureWidget(new KLLMConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Notification History"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));

    mKLLMConfigureWidget->setObjectName(QLatin1StringView("mKLLMConfigureWidget"));
    mainLayout->addWidget(mKLLMConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QLatin1StringView("buttonBox"));
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &KLLMConfigureDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &KLLMConfigureDialog::reject);
}

KLLMConfigureDialog::~KLLMConfigureDialog() = default;

#include "moc_KLLMConfigureDialog.cpp"
