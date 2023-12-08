// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QComboBox>

namespace KLLMWidgets
{
class KLLMModelComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit KLLMModelComboBox(QWidget *parent = nullptr);
    ~KLLMModelComboBox() override;
};
}
