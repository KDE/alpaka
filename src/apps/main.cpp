// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>

#include "ChatModel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kandalf");

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));

    KAboutData about(QStringLiteral("kandalf"),
                     i18n("Kandalf"),
                     QStringLiteral(KANDALF_VERSION_STR),
                     i18n("Ollama client"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2023 Loren Burkholder"));
    about.addAuthor(i18n("Loren Burkholder"), i18n("Maintainer"), QStringLiteral("computersemiexpert@outlook.com"));
    about.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    about.setOrganizationDomain("kde.org");

    KAboutData::setApplicationData(about);

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    QQmlApplicationEngine engine;

    qmlRegisterType<ChatModel>("org.kde.kandalf", 0, 1, "ChatModel");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
