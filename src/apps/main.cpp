// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KAboutData>
#include <KCrash>
#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>

#include "ChatModel.h"
#include "alpaka-version.h"
#include "controller.h"
#include "kllmcoresettings.h"
#include "windowcontroller.h"

// signal handler for SIGINT & SIGTERM
#ifdef Q_OS_UNIX
#include <KSignalHandler>
#include <signal.h>
#include <unistd.h>
#endif

static QWindow *windowFromEngine(QQmlApplicationEngine *engine)
{
    const auto rootObjects = engine->rootObjects();
    auto *window = qobject_cast<QQuickWindow *>(rootObjects.first());
    Q_ASSERT(window);
    return window;
}

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("alpaka"));

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));

    KAboutData about(QStringLiteral("alpaka"),
                     i18n("Alpaka"),
                     QStringLiteral(ALPAKA_VERSION_STRING),
                     i18n("Ollama client"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2023-2025 Loren Burkholder"));
    about.addAuthor(i18n("Loren Burkholder"), i18n("Maintainer"), QStringLiteral("computersemiexpert@outlook.com"));
    about.addCredit(i18n("Laurent Montel"), i18n("Developer"), QStringLiteral("montel@kde.org"));
    about.setOrganizationDomain("kde.org");

    KAboutData::setApplicationData(about);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("alpaka")));
    KCrash::initialize();

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    auto settings = KLLMCoreSettings::self();
    auto &controller = Controller::instance();
    qmlRegisterSingletonInstance("org.kde.alpaka", 1, 0, "AlpakaSettings", settings);
    qmlRegisterSingletonInstance("org.kde.alpaka", 1, 0, "Controller", &controller);
    qmlRegisterType<ChatModel>("org.kde.alpaka", 0, 1, "ChatModel");

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
    engine.loadFromModule("org.kde.alpaka", "Main");

    app.connect(&app, &QApplication::aboutToQuit, KLLMCoreSettings::self(), &KLLMCoreSettings::save);

    QWindow *window = windowFromEngine(&engine);

    WindowController::instance().setWindow(window);
    WindowController::instance().restoreGeometry();
#ifdef Q_OS_UNIX
    /**
     * Set up signal handler for SIGINT and SIGTERM
     */
    KSignalHandler::self()->watchSignal(SIGINT);
    KSignalHandler::self()->watchSignal(SIGTERM);
    QObject::connect(KSignalHandler::self(), &KSignalHandler::signalReceived, &app, [](int signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            printf("Shutting down...\n");
            QApplication::exit();
        }
    });
#endif

    return app.exec();
}
