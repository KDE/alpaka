# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_executable(kognos)

configure_file(config-kognos.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-kognos.h)

target_sources(kognos PRIVATE
    ChatModel.cpp
    ChatModel.h
    main.cpp

    systemtray.h
    systemtray.cpp

    windowcontroller.cpp
    windowcontroller.h

    controller.h
    controller.cpp

    resources.qrc
)

target_link_libraries(kognos
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
    KLLMCore
    KF6::StatusNotifierItem
    KF6::WindowSystem
)

kconfig_add_kcfg_files(kognos GENERATE_MOC kognos.kcfgc)

target_include_directories(kognos PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS kognos ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
