// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QObject>

class KLLMConfigureDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit KLLMConfigureDialogTest(QObject *parent = nullptr);
    ~KLLMConfigureDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
