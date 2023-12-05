# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_executable(kandalf)

target_sources(kandalf PRIVATE
    ChatModel.cpp
    ChatModel.h
    kllm/KLLMContext.cpp
    kllm/KLLMContext.h
    kllm/KLLMInterface.cpp
    kllm/KLLMInterface.h
    kllm/KLLMReply.cpp
    kllm/KLLMReply.h
    kllm/KLLMRequest.cpp
    kllm/KLLMRequest.h
    apps/main.cpp

    resources.qrc
)

target_link_libraries(kandalf
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::QuickControls2
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::Kirigami2
)

install(TARGETS kandalf ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
