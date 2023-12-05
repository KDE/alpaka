# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

cmake_minimum_required(VERSION 3.16)
project(Kandalf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

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

find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Gui
    QuickControls2
    Network
)

find_package(KF${QT_MAJOR_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Kirigami2
    I18n
)

ecm_set_disabled_deprecation_versions(QT 6.6  KF 5.246.0)

add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
