cmake_minimum_required(VERSION 3.16)

project(MyMusic VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Quick QuickControls2 REQUIRED)

qt_add_executable(MyMusic
        MANUAL_FINALIZATION
        main.cpp
        KeyFilter.cpp
        FileSystem.cpp
        Controller.cpp
        Settings.cpp
        )

qt_add_qml_module(MyMusic
        URI MyMusic
        VERSION 1.0

        QML_FILES
        main.qml
        MusicPlayer.qml
        FileListView.qml
        NoteListItem.qml
        MusicLyrics.qml

        RESOURCES
        settings_64x64.png
        music_128x128.png
        note_64x64.png
        left-circle_64x64.png
        pause-circle_64x64.png
        play-circle_64x64.png
        check_64x64.png
        music_64x64.png
        folder_64x64.png
        right-circle_64x64.png
        disk_600x600.jpeg
        )

set_target_properties(MyMusic PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER cn.net.pikachu
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android
        )

target_link_libraries(MyMusic
        PRIVATE Qt6::Quick Qt6::QuickControls2)

install(TARGETS MyMusic
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_target(MyMusic)
