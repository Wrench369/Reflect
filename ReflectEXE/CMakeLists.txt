cmake_minimum_required(VERSION 3.10)
project(ReflectEXE VERSION 1.0)

# 设置 C++ 特性
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}")

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "../Reflect/src/*.cpp")
file(GLOB_RECURSE HEADERS "../Reflect/inc/*.h")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 设置包含目录
target_include_directories(${PROJECT_NAME} PUBLIC "../Reflect/inc")

# 针对不同配置的设置
set(CMAKE_CONFIGURATION_TYPES Debug Release Dist)

# Debug 配置
target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
target_compile_options(${PROJECT_NAME} PUBLIC -g)
# Release 配置
target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE)
target_compile_options(${PROJECT_NAME} PUBLIC -O2)
# Dist 配置
target_compile_definitions(${PROJECT_NAME} PUBLIC DIST)
target_compile_options(${PROJECT_NAME} PUBLIC -O3)
