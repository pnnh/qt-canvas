cmake_minimum_required(VERSION 3.15)

SET(PROJECT_NAME qt-canvas)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

find_package(Qt6 COMPONENTS Core REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        src/stele/stele.cpp src/stele/stele.h
        src/md5.cc src/md5.h
        src/wasm/wasm.h
        ${PROJECT_SOURCE_DIR}/src/markdown/mdtransform.hpp
        ${PROJECT_SOURCE_DIR}/src/markdown/mdtransform.cpp)

qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# 当编译WebAssembly时修改链接参数，否则会报初始内存太小错误
if (EMSCRIPTEN)
    target_link_options(${PROJECT_NAME} PRIVATE -s TOTAL_MEMORY=32MB)
endif ()

qt_finalize_executable(${PROJECT_NAME})
