cmake_minimum_required(VERSION 3.6)
project(krash-bot LANGUAGES CXX)
include_directories(/usr/include/ImageMagick-7)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic-errors)

include(cmake/DPP.cmake)
include(cmake/dotenv.cmake)
include(cmake/drogon.cmake)
