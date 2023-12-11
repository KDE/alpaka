# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_executable(kandalf
    controller.h controller.cpp)

configure_file(config-kandalf.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-kandalf.h)

target_sources(kandalf PRIVATE
    ChatModel.cpp
    ChatModel.h
    main.cpp

    systemtray.h
    systemtray.cpp

    windowcontroller.cpp
    windowcontroller.h

    resources.qrc
)

target_link_libraries(kandalf
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

kconfig_add_kcfg_files(kandalf GENERATE_MOC kandalf.kcfgc)

target_include_directories(kandalf PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS kandalf ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
