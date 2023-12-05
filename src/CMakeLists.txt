add_executable(kandalf)

target_sources(kandalf PRIVATE
    ChatModel.cpp
    ChatModel.h
    KLLMContext.h
    KLLMInterface.cpp
    KLLMInterface.h
    KLLMReply.cpp
    KLLMReply.h
    KLLMRequest.cpp
    KLLMRequest.h
    main.cpp

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
