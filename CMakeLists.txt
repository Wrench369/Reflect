cmake_minimum_required(VERSION 3.10)
project(Reflect)

# 设置 C++ 特性
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 将 -fpermissive 选项添加到编译器选项中
add_compile_options(-fpermissive)

add_definitions(-DREFLECT_TYPE_INFO_ENABLED)

# 设置默认配置
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug, Release, Dist" FORCE)
endif()

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/%{cfg.system}-%{cfg.architecture}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/%{cfg.system}-%{cfg.architecture}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/%{cfg.system}-%{cfg.architecture}")

# 添加宏定义
# add_compile_definitions(REFLECT_TYPE_INFO_ENABLED)

# 添加子目录
add_subdirectory(Reflect)
add_subdirectory(ReflectEXE)
add_subdirectory(ReflectTest)
add_subdirectory(Test)

