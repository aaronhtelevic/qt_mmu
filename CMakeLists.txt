cmake_minimum_required(VERSION 3.16)
project(QtMultimediaApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets Pdf PdfWidgets Multimedia MultimediaWidgets)

add_executable(QtMultimediaApp main.cpp)

target_link_libraries(QtMultimediaApp PRIVATE
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::Pdf
    Qt6::Multimedia
)

# Enable Qt6's automatic MOC, UIC, and RCC handling
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

