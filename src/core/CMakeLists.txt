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
    KLLMContext.h

    KLLMInterface.cpp
    KLLMInterface.h

    KLLMReply.cpp
    KLLMReply.h

    KLLMRequest.cpp
    KLLMRequest.h

    KLLMConstants.cpp
    KLLMConstants.h
)

kconfig_add_kcfg_files(KLLMCore GENERATE_MOC kllmcoresettings.kcfgc)

ecm_qt_declare_logging_category(KLLMCore HEADER kllmcore_debug.h IDENTIFIER KLLMCORE_LOG CATEGORY_NAME org.kde.kllm.core
        DESCRIPTION "kllmcore" EXPORT KLLMCORE)

generate_export_header(KLLMCore
    BASE_NAME kllmcore
)

target_link_libraries(KLLMCore
    Qt::Network
    KF6::I18n
    KF6::ConfigGui
    KF6::CoreAddons
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
    KLLMConstants

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
    ${CMAKE_CURRENT_BINARY_DIR}/kllmcoresettings.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KLLMCore/kllmcore
    COMPONENT Devel
)

if (BUILD_QCH)
    ecm_add_qch(
        kllmcore_QCH
        NAME KLLM
        BASE_NAME KLLMCore
        VERSION ${KLLMCORE_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kllmcore_Camelcase_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
        Qt6Core_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        KLLMCORE_EXPORT
        KLLMCORE_DEPRECATED_EXPORT
        KLLMCORE_DEPRECATED
        "KLLMCORE_DEPRECATED_VERSION(x, y, t)"
        "KLLMCORE_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
        "KLLMCORE_ENUMERATOR_DEPRECATED_VERSION(x, y, t)"
        "KLLMCORE_ENUMERATOR_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
    ecm_install_qch_export(
        TARGETS kllmcore_QCH
        FILE KLLMCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KLLMCoreQchTargets.cmake\")")

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

if(BUILD_TESTING)
   add_subdirectory(tests)
   add_subdirectory(autotests)
endif()
