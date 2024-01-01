// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "kllmconstantstest.h"
#include "KLLMConstants.h"
#include <QTest>

QTEST_GUILESS_MAIN(KLLMConstantsTest)
KLLMConstantsTest::KLLMConstantsTest(QObject *parent)
    : QObject{parent}
{
}

void KLLMConstantsTest::shouldHaveDefaultValues()
{
    QCOMPARE(KLLMCore::ollamaUrl(), QUrl(QStringLiteral("http://0.0.0.0:11434")));
}

#include "moc_kllmconstantstest.cpp"
