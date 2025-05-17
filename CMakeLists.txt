cmake_minimum_required(VERSION 3.16)

project(qchess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqchess
    src/main.cpp
    src/gamestate.h
    src/pieces.h
    src/validator.cpp
    src/validator.h
    src/moveexecutor.cpp
    src/moveexecutor.h
)

qt_add_qml_module(appqchess
    URI qchess
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        src/controller.cpp
        src/controller.h
        src/board.cpp
        src/board.h
        src/square.cpp
        src/square.h
    RESOURCES
        resources/Alpha.ttf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqchess PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqchess
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqchess
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appqchess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
