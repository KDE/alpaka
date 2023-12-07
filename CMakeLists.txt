# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

cmake_minimum_required(VERSION 3.16)
project(kandalf VERSION 0.1 LANGUAGES CXX)

set(KF_MIN_VERSION "5.246.0")
set(QT_MIN_VERSION "6.6")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMAddTests)
include(KDEGitCommitHooks)
include(KDEClangFormat)
include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMSetupVersion)
include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Gui
    QuickControls2
    Network
    Widgets
)

find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    Kirigami2
    I18n
)

ecm_set_disabled_deprecation_versions(QT ${QT_MIN_VERSION} KF ${KF_MIN_VERSION})

ecm_setup_version(${PROJECT_VERSION}
        VARIABLE_PREFIX KANDALF
        VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/kandalf-version.h
)

if(BUILD_TESTING)
    add_definitions(-DBUILD_TESTING)
endif()

add_subdirectory(src)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KLLMCore")
message("${CMAKECONFIG_INSTALL_DIR}")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS libkllmcore_QCH
        FILE KLLMCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KLLMCoreQchTargets.cmake\")")
endif()

ecm_qt_install_logging_categories(EXPORT KLLMCORE FILE kllm.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
