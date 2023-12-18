// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KAboutData>
#include <KCrash>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>

#include "ChatModel.h"
#include "controller.h"
#include "kllmcoresettings.h"
#include "kognos-version.h"
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

    KLocalizedString::setApplicationDomain("kognos");

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));

    KCrash::initialize();
    KAboutData about(QStringLiteral("kognos"),
                     i18n("Kognos"),
                     QStringLiteral(KOGNOS_VERSION_STRING),
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

    auto settings = KLLMCoreSettings::self();
    auto &controller = Controller::instance();
    qmlRegisterSingletonInstance("org.kde.kognos", 1, 0, "KognosSettings", settings);
    qmlRegisterSingletonInstance("org.kde.kognos", 1, 0, "Controller", &controller);
    qmlRegisterType<ChatModel>("org.kde.kognos", 0, 1, "ChatModel");

    Controller::instance();
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [] {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/org/kde/kognos/qml/main.qml")));

    app.connect(&app, &QApplication::aboutToQuit, KLLMCoreSettings::self(), &KLLMCoreSettings::save);

    QWindow *window = windowFromEngine(&engine);

    WindowController::instance().setWindow(window);
    WindowController::instance().restoreGeometry();

    return app.exec();
}
