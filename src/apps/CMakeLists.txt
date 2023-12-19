# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_executable(alpaka)

configure_file(config-alpaka.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-alpaka.h)

target_sources(alpaka PRIVATE
    main.cpp

    ChatModel.cpp
    ChatModel.h

    windowcontroller.cpp
    windowcontroller.h

    controller.h
    controller.cpp

    systemtray.h
    systemtray.cpp
)

qt_add_qml_module(alpaka
    URI org.kde.alpaka
    VERSION 1.0
    QML_FILES
        qml/GeneralSettingsPage.qml
        qml/LLMSettingsPage.qml
        qml/main.qml
        qml/OllamaSettingsPage.qml
        qml/SettingsPage.qml
        qml/TypingIndicator.qml
)

target_link_libraries(alpaka PRIVATE
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    KF6::I18n
    KF6::Kirigami2
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::Crash
    KLLMCore
    KF6::StatusNotifierItem
    KF6::WindowSystem
)

target_include_directories(alpaka PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS alpaka ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
