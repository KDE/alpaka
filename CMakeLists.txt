# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

cmake_minimum_required(VERSION 3.16)
project(alpaka VERSION 0.1.1 LANGUAGES CXX)

set(KF_MIN_VERSION "6.0.0")
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

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Gui
    QuickControls2
    Network
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    Kirigami
    I18n
    StatusNotifierItem
    WindowSystem
    XmlGui
    Crash
)

# Remove it on android etc.
if (NOT ANDROID)
    set(HAVE_WINDOWSYSTEM TRUE)
endif()
if (NOT ANDROID AND NOT WIN32 AND NOT APPLE)
    set(HAVE_X11 TRUE)
endif()
ecm_set_disabled_deprecation_versions(QT ${QT_MIN_VERSION} KF ${KF_MIN_VERSION})

ecm_setup_version(${PROJECT_VERSION}
        VARIABLE_PREFIX ALPAKA
        VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/alpaka-version.h
)

if(BUILD_TESTING)
    add_definitions(-DBUILD_TESTING)
    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Test)
endif()

add_subdirectory(src)

install(PROGRAMS org.kde.alpaka.desktop DESTINATION ${KDE_INSTALL_APPDIR})

ecm_qt_install_logging_categories(EXPORT KLLMCORE FILE kllm.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
