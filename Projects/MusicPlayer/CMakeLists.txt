cmake_minimum_required(VERSION 3.15)

project(MyMusic LANGUAGES CXX VERSION "1.0.0")

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
find_package(Qt6 COMPONENTS Core Quick Qml QuickControls2 REQUIRED)
add_library(MyMusic_static STATIC)
target_sources(MyMusic_static PRIVATE
        Controller.cpp Controller.h
        FileSystem.cpp FileSystem.h
        KeyFilter.cpp KeyFilter.h
        Settings.cpp Settings.h
        )
target_link_libraries(MyMusic_static PRIVATE Qt::Gui Qt::QuickControls2)