cmake_minimum_required(VERSION 3.16)

project(sorting-visualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(APPLE)
  set(SDL2_FRAMEWORK_PATH "${CMAKE_SOURCE_DIR}/SDL2/SDL2.framework")
  include_directories("${SDL2_FRAMEWORK_PATH}/Headers")
  link_directories("${SDL2_FRAMEWORK_PATH}")
elseif(WIN32)
  set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SDL2/include")
  set(SDL2_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/SDL2/lib")
  include_directories(${SDL2_INCLUDE_DIR})
endif()

set(PROJECT_SOURCES
    "Source Files/main.cpp"

    "Source Files/mainwindow.cpp"
    "Header Files/mainwindow.h"

    "Source Files/soundplayer.cpp"
    "Header Files/soundplayer.h"

    "Header Files/secondwindow.h"
    "Source Files/secondwindow.cpp"

    "Header Files/soundplayer.h"
    "Source Files/soundplayer.cpp"

    "Header Files/algorithms.h"
    "Source Files/algorithms.cpp"

    "Source Files/Data Structures/vector.cpp"
    "Header Files/Data Structures/vector.h"

    "Header Files/Data Structures/graph.h"
    "Source Files/Data Structures/graph.cpp"

    "Header Files/Data Structures/pair.h"
    "Source Files/Data Structures/pair.cpp"

    "Header Files/Data Structures/stack.h"
    "Source Files/Data Structures/stack.cpp"

    "Header Files/Data Structures/queue.h"
    "Source Files/Data Structures/queue.cpp"

    "Header Files/Data Structures/dlist.h"
    "Source Files/Data Structures/dlist.cpp"

    "Header Files/Data Structures/priorityqueue.h"
    "Source Files/Data Structures/priorityqueue.cpp"

    "Header Files/Data Structures/list.h"
    "Source Files/Data Structures/list.cpp"

    mainwindow.ui
    secondwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(sorting-visualizer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(sorting-visualizer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(sorting-visualizer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(APPLE)
  target_link_libraries(sorting-visualizer PRIVATE "${SDL2_FRAMEWORK_PATH}")
elseif(WIN32)
  set(SDL2_LIBRARIES
    "${SDL2_LIBRARY_DIR}/libSDL2.dll.a"
    "${SDL2_LIBRARY_DIR}/libSDL2main.a"
  )
  target_link_libraries(sorting-visualizer PRIVATE ${SDL2_LIBRARIES})
endif()

target_link_libraries(sorting-visualizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.sorting-visualizer)
endif()

set_target_properties(sorting-visualizer PROPERTIES
    ${BUNDLE_ID_OPTION}
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/Frameworks/SDL2.framework
    DESTINATION ${CMAKE_BINARY_DIR}/Release/${PROJECT_NAME}.app/Contents/Frameworks
)

include(GNUInstallDirs)
install(TARGETS sorting-visualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(sorting-visualizer)
endif()
