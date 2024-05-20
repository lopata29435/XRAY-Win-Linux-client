cmake_minimum_required(VERSION 3.5)

project(CrossBorder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск компонентов Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Svg) # Добавляем Core сюда
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network Svg) # И здесь

#Добавление ресурсов
qt6_add_resources(RESOURCES res/res.qrc)

# Список исходных файлов проекта
set(PROJECT_SOURCES main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    3rdparty/v2/components/proxy/QvProxyConfigurator.cpp
    3rdparty/v2/components/proxy/QvProxyConfigurator.hpp
    3rdparty/wrapper.hpp
    components/powerbutton.h
    components/powerbutton.cpp
    ${RESOURCES}
    )

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CrossBorder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

# Добавление директории для поиска заголовочных файлов
target_include_directories(CrossBorder PRIVATE "3rdparty/v2/components/proxy/")

# Цель проекта: связывание с библиотеками Qt
target_link_libraries(CrossBorder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Wininet Rasapi32 Qt${QT_VERSION_MAJOR}::Svg) # Добавляем Core здесь

# Настройки цели проекта для разных платформ
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CrossBorder)
endif()
set_target_properties(CrossBorder PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка исполняемого файла
include(GNUInstallDirs)
install(TARGETS CrossBorder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Завершение сборки для Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CrossBorder)
endif()
