# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KLLMWidgets")

ecm_setup_version(PROJECT VARIABLE_PREFIX KLLMWIDGETS
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kllmwidgets_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KLLMWidgetsConfigVersion.cmake"
    SOVERSION 0
)

add_library(KLLMWidgets)

set_target_properties(KLLMWidgets PROPERTIES
    VERSION ${KLLMWIDGETS_VERSION}
    SOVERSION ${KLLMWIDGETS_SOVERSION}
    EXPORT_NAME KLLMWidgets
)

target_sources(KLLMWidgets PRIVATE
    KLLMModelComboBox.h
    KLLMModelComboBox.cpp

    KLLMActionMenu.h
    KLLMActionMenu.cpp

    KLLMConfigureWidget.h
    KLLMConfigureWidget.cpp

    KLLMConfigureDialog.h
    KLLMConfigureDialog.cpp
)

ecm_qt_declare_logging_category(KLLMWidgets HEADER kllmwidgets_debug.h IDENTIFIER KLLMWIDGETS_LOG CATEGORY_NAME org.kde.kllm.core
        DESCRIPTION "kllmwidgets" EXPORT KLLMWIDGETS)

generate_export_header(KLLMWidgets
    BASE_NAME kllmwidgets
)

target_link_libraries(KLLMWidgets
    KLLMCore
    Qt::Widgets
    KF6::XmlGui
)

set_target_properties(KLLMWidgets
    PROPERTIES OUTPUT_NAME kllmwidgets VERSION ${KLLMWIDGETS_VERSION} SOVERSION ${KLLMWIDGETS_SOVERSION}
)

install(TARGETS KLLMWidgets EXPORT KLLMWidgetsTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(kllmwidgets_Camelcase_HEADERS
    ORIGINAL CAMELCASE
    HEADER_NAMES
    KLLMModelComboBox
    KLLMActionMenu
    KLLMConfigureWidget
    KLLMConfigureDialog

    REQUIRED_HEADERS kllmwidgets_HEADERS
    PREFIX KLLMWidgets
)

install(FILES
    ${kllmwidgets_Camelcase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KLLMWidgets
    COMPONENT Devel
)

install(FILES
    ${kllmwidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kllmwidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KLLMWidgets/kllmwidgets
    COMPONENT Devel
)

if (BUILD_QCH)
    ecm_add_qch(
        kllmwidgets_QCH
        NAME KLLM
        BASE_NAME KLLMWidgets
        VERSION ${KLLMWIDGETS_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${kllmwidgets_Camelcase_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
        Qt6Core_QCH
        Qt6Widgets_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        KLLMWIDGETS_EXPORT
        KLLMWIDGETS_DEPRECATED_EXPORT
        KLLMWIDGETS_DEPRECATED
        "KLLMWIDGETS_DEPRECATED_VERSION(x, y, t)"
        "KLLMWIDGETS_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
        "KLLMWIDGETS_ENUMERATOR_DEPRECATED_VERSION(x, y, t)"
        "KLLMWIDGETS_ENUMERATOR_DEPRECATED_VERSION_BELATED(x, y, tx, ty, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
    ecm_install_qch_export(
        TARGETS kllmwidgets_QCH
        FILE KLLMWidgetsQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KLLMWidgetsQchTargets.cmake\")")

endif()


configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KLLMWidgetsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMWidgetsConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMWidgetsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KLLMWidgetsConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KLLMWidgetsTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KLLMWidgetsTargets.cmake
    )

if(BUILD_TESTING)
   add_subdirectory(tests)
   add_subdirectory(autotests)
endif()
