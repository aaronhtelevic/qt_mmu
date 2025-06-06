cmake_minimum_required(VERSION 3.16)
project(QtMultimediaApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt6's automatic MOC, UIC, and RCC handling
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets Pdf PdfWidgets Multimedia MultimediaWidgets)

message(STATUS "Using Qt version ${Qt6Core_VERSION}")
message(STATUS "Qt6Core_DIR: ${Qt6Core_DIR}")

set(HEADERS
    include/video_player.hpp
    include/pdf_viewer.hpp
)
set(SOURCES
    src/main.cpp
    src/video_player.cpp
    src/pdf_viewer.cpp
)

add_executable(QtMultimediaApp ${HEADERS} ${SOURCES})
include_directories(include)

qt_add_resources(RESOURCES resources.qrc)
target_sources(QtMultimediaApp PRIVATE ${RESOURCES})

target_link_libraries(QtMultimediaApp PRIVATE
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::Pdf
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

install(TARGETS QtMultimediaApp
    RUNTIME DESTINATION bin
)
