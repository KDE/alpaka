cmake_minimum_required(VERSION 3.16)
project(Kandalf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Test
    Gui
    QuickControls2
    Widgets
    Network
)

find_package(KF${QT_MAJOR_VERSION} REQUIRED COMPONENTS
    Kirigami2
    I18n
    CoreAddons
)

find_package(QCoro${QT_MAJOR_VERSION} 0.9.0 REQUIRED COMPONENTS Core Network)

add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
