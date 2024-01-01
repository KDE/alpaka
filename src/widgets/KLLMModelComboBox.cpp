// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMModelComboBox.h"
using namespace KLLMWidgets;
KLLMModelComboBox::KLLMModelComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

KLLMModelComboBox::~KLLMModelComboBox() = default;

void KLLMModelComboBox::setModels(const QStringList &models)
{
    addItems(models);
}

#include "moc_KLLMModelComboBox.cpp"
