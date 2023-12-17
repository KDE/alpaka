// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "kllmresultwidgettest.h"
#include "KLLMResultWidget.h"
#include <QPlainTextEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(KLLMResultWidgetTest)
KLLMResultWidgetTest::KLLMResultWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KLLMResultWidgetTest::shouldHaveDefaultValues()
{
    KLLMWidgets::KLLMResultWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mResult = w.findChild<QPlainTextEdit *>(QStringLiteral("mResult"));
    QVERIFY(mResult);
    QVERIFY(mResult->isReadOnly());
}
