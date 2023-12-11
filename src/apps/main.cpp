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
#include "kandalf-version.h"
#include "kandalf.h"
#include "windowcontroller.h"

static QWindow *windowFromEngine(QQmlApplicationEngine *engine)
{
    const auto rootObjects = engine->rootObjects();
    auto *window = qobject_cast<QQuickWindow *>(rootObjects.first());
    Q_ASSERT(window);
    return window;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kandalf");

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));

    KAboutData about(QStringLiteral("kandalf"),
                     i18n("Kandalf"),
                     QStringLiteral(KANDALF_VERSION_STRING),
                     i18n("Ollama client"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2023 Loren Burkholder"));
    about.addAuthor(i18n("Loren Burkholder"), i18n("Maintainer"), QStringLiteral("computersemiexpert@outlook.com"));
    about.addCredit(i18n("Laurent Montel"), i18n("Developer"), QStringLiteral("montel@kde.org"));
    about.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    about.setOrganizationDomain("kde.org");

    KAboutData::setApplicationData(about);

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    QQmlApplicationEngine engine;

    auto settings = KandalfSettings::self();
    qmlRegisterSingletonInstance("org.kde.kandalf", 1, 0, "KandalfSettings", settings);
    qmlRegisterType<ChatModel>("org.kde.kandalf", 0, 1, "ChatModel");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    app.connect(&app, &QApplication::aboutToQuit, settings, &KandalfSettings::save);

    QWindow *window = windowFromEngine(&engine);

    WindowController::instance().setWindow(window);
    WindowController::instance().restoreGeometry();

    return app.exec();
}
