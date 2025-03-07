cmake_minimum_required(VERSION 3.19)
project(Chess LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_resources(RESOURCES icons.qrc)

qt_add_executable(Chess
    WIN32 MACOSX_BUNDLE
    main.cpp
    interface.cpp
    interface.h
    interface.ui
    ${RESOURCES}
    figure.h figure.cpp
    king.h king.cpp
    queen.h queen.cpp
    bishop.h bishop.cpp
    rook.h rook.cpp
    knight.h knight.cpp
    pawn.h pawn.cpp
    chessboard.h chessboard.cpp
    player.h player.cpp
    game.h game.cpp
    chesssquare.h chesssquare.cpp
)

target_link_libraries(Chess
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Chess
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Chess
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
