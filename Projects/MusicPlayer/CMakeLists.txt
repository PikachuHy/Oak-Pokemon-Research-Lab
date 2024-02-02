cmake_minimum_required(VERSION 3.16)

project(MyMusic VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
    message(STATUS "Configuring default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
else ()
    message(STATUS "Configuring CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif ()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    if (APPLE)
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/MyNotes.app/Contents/MacOS)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/MyNotes.app/Contents/PlugIns)
    endif ()
endif ()

find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup(REQUIRES 6.7)

qt_add_executable(MyMusic
    main.cpp
    KeyFilter.cpp
    FileSystem.cpp
    Controller.cpp
    Settings.cpp
)

qt_add_qml_module(MyMusic
    URI MyMusic
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES FileListView.qml
    QML_FILES MusicPlayer.qml
    QML_FILES NoteListItem.qml
    QML_FILES MusicLyrics.qml
    RESOURCES disk_600x600.png
    RESOURCES settings_64x64.png
    RESOURCES left-circle_64x64.png
    RESOURCES right-circle_64x64.png
    RESOURCES play-circle_64x64.png
    RESOURCES pause-circle_64x64.png
    RESOURCES note_64x64.png
    RESOURCES folder_64x64.png
    RESOURCES check_64x64.png

    QML_FILES
    SnowyEffect/export/SnowyEffect.qml

    RESOURCES
    SnowyEffect/export/fog.png
    SnowyEffect/export/snowyeffect.vert.qsb
    SnowyEffect/export/snowyeffect.frag.qsb
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(MyMusic PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER cn.net.pikachu.MyMusic
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android
)

target_link_libraries(MyMusic
    PRIVATE Qt6::Quick Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS MyMusic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
