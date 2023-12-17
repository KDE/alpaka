// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QObject>

class KLLMResultWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KLLMResultWidgetTest(QObject *parent = nullptr);
    ~KLLMResultWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
