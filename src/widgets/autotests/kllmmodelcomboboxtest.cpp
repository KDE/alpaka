// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
#include "kllmmodelcomboboxtest.h"
#include <KLLMModelComboBox.h>
#include <QTest>

QTEST_MAIN(KLLMModelComboBoxTest)
KLLMModelComboBoxTest::KLLMModelComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

KLLMModelComboBoxTest::~KLLMModelComboBoxTest() = default;

void KLLMModelComboBoxTest::shouldHaveDefaultValues()
{
    KLLMWidgets::KLLMModelComboBox w;
    QCOMPARE(w.count(), 0);
}

#include "moc_kllmmodelcomboboxtest.cpp"
