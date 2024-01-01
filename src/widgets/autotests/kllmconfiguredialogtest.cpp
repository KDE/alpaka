// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "kllmconfiguredialogtest.h"
#include "KLLMConfigureDialog.h"
#include <QTest>
QTEST_MAIN(KLLMConfigureDialogTest)
KLLMConfigureDialogTest::KLLMConfigureDialogTest(QObject *parent)
    : QObject{parent}
{
}

void KLLMConfigureDialogTest::shouldHaveDefaultValues()
{
    KLLMWidgets::KLLMConfigureDialog w;
    // TODO
}

#include "moc_kllmconfiguredialogtest.cpp"
