// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
#pragma once

#include <QObject>

class KLLMModelComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit KLLMModelComboBoxTest(QObject *parent = nullptr);
    ~KLLMModelComboBoxTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
