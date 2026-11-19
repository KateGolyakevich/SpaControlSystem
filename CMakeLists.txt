cmake_minimum_required(VERSION 3.16)
project(SpaControlSystem VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt" "/opt/homebrew/opt/qt6" "/opt/homebrew/Cellar/qtbase/6.12.0_2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Автоматический поиск файлов
file(GLOB_RECURSE PROJECT_HEADERS "include/*.h")
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)

# ТОЧНЫЕ ПУТИ К НАЙДЕННЫМ ФАЙЛАМ QT НА ТВОЕМ ДИСКЕ:
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        /opt/homebrew/Cellar/qtbase/6.12.0_2/include
        /opt/homebrew/Cellar/qtbase/6.12.0_2/include/QtCore
        /opt/homebrew/Cellar/qtbase/6.12.0_2/include/QtGui
        /opt/homebrew/Cellar/qtbase/6.12.0_2/include/QtWidgets
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

# Копирование базы данных
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})