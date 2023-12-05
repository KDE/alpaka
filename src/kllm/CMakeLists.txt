# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_library(libkllmcore)

target_sources(libkllmcore PRIVATE
    KLLMContext.cpp
    KLLMContext.h
    KLLMInterface.cpp
    KLLMInterface.h
    KLLMReply.cpp
    KLLMReply.h
    KLLMRequest.cpp
    KLLMRequest.h
)

generate_export_header(libkllmcore BASE_NAME libkllmcore)


target_link_libraries(libkllmcore
    Qt::Network
)
