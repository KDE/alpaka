# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

add_library(libkllmcore)

set_target_properties(libkllmcore PROPERTIES
    VERSION "${KANDALF_VERSION}"
)

set(libkllmcore_HEADERS
    KLLMContext.h
    KLLMInterface.h
    KLLMReply.h
    KLLMRequest.h
)

target_sources(libkllmcore PRIVATE
    KLLMContext.cpp
    KLLMInterface.cpp
    KLLMReply.cpp
    KLLMRequest.cpp

    ${libkllmcore_HEADERS}
)

generate_export_header(libkllmcore BASE_NAME libkllmcore)

target_link_libraries(libkllmcore
    Qt::Network
)

set_target_properties(libkllmcore
    PROPERTIES OUTPUT_NAME kllmcore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
)

if (BUILD_QCH)
    ecm_add_qch(
        libkllmcore_QCH
        NAME KLLM
        BASE_NAME KLLMCore
        VERSION ${kandalf_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${libkllmcore_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            LIBKLLMCORE_EXPORT
            LIBKLLMCORE_DEPRECATED_EXPORT
            LIBKLLMCORE_DEPRECATED
            "LIBKLLMCORE_DEPRECATED_VERSION(x, y, t)"
            "LIBKLLMCORE_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
            "LIBKLLMCORE_ENUMERATOR_DEPRECATED_VERSION(x, y, t)"
            "LIBKLLMCORE_ENUMERATOR_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

install(TARGETS libkllmcore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
