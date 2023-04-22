cmake_minimum_required(VERSION 3.7)
set(TARGET_NAME krabby)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE FORMS "*.ui")
file(GLOB_RECURSE RESOURCES "*.qrc")

set(QT Core Gui Widgets Multimedia)
find_package(Qt6 REQUIRED ${QT})
qt_add_executable(krabby
    MANUAL_FINALIZATION
    ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES}
)
target_link_libraries(krabby PRIVATE Qt::Widgets Qt::Multimedia)

set_target_properties(krabby PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER krabby.pikachu.net.cn
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_ICON_FILE krabby.icns
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
qt_finalize_executable(krabby)
