# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KLLMCore")

ecm_setup_version(PROJECT VARIABLE_PREFIX KLLMCORE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kllmcore_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KLLMCoreConfigVersion.cmake"
    SOVERSION 0
)

add_library(KLLMCore)


set_target_properties(KLLMCore PROPERTIES
    VERSION ${KLLMCORE_VERSION}
    SOVERSION ${KLLMCORE_SOVERSION}
    EXPORT_NAME KLLMCore
)

target_sources(KLLMCore PRIVATE
    KLLMContext.cpp
    KLLMInterface.cpp
    KLLMReply.cpp
    KLLMRequest.cpp

    KLLMContext.h
    KLLMInterface.h
    KLLMReply.h
    KLLMRequest.h
)

generate_export_header(KLLMCore
    BASE_NAME kllmcore
)

target_link_libraries(KLLMCore
    Qt::Network
)

set_target_properties(KLLMCore
    PROPERTIES OUTPUT_NAME kllmcore VERSION ${KLLMCORE_VERSION} SOVERSION ${KLLMCORE_SOVERSION}
)

install(TARGETS KLLMCore EXPORT KLLMCoreTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(kllmcore_Camelcase_HEADERS
    ORIGINAL CAMELCASE
    HEADER_NAMES
    KLLMInterface
    KLLMReply

    REQUIRED_HEADERS kllmcore_HEADERS
    PREFIX KLLMCore
)

install(FILES
    ${kllmcore_Camelcase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KLLMCore
    COMPONENT Devel
)

install(FILES
    ${kllmcore_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kllmcore_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KLLMCore/kllmcore
    COMPONENT Devel
)

if (BUILD_QCH)
    ecm_add_qch(
        libkllmcore_QCH
        NAME KLLM
        BASE_NAME KLLMCore
        VERSION ${kandalf_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kllmcore_Camelcase_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
        Qt6Core_QCH
        Qt6Gui_QCH
        Qt6Widgets_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        KLLMCORE_EXPORT
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


configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KLLMCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMCoreConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMCoreConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KLLMCoreTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KLLMCoreTargets.cmake
    )
