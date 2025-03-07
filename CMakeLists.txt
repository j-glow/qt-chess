cmake_minimum_required(VERSION 3.19)
project(Chess LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_resources(RESOURCES icons.qrc)

qt_add_executable(Chess
    WIN32 MACOSX_BUNDLE
    main.cpp
    includes/interface.h    interface.cpp
    interface.ui
    ${RESOURCES}

    includes/figure.h       figure.cpp
    includes/chessboard.h   chessboard.cpp
    includes/player.h       player.cpp
    includes/game.h         game.cpp
    includes/chesssquare.h  chesssquare.cpp

    includes/king.h         figures/king.cpp
    includes/queen.h        figures/queen.cpp
    includes/bishop.h       figures/bishop.cpp
    includes/rook.h         figures/rook.cpp
    includes/knight.h       figures/knight.cpp
    includes/pawn.h         figures/pawn.cpp
)

target_include_directories(Chess
    PRIVATE
        includes
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
