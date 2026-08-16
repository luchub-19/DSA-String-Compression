cmake_minimum_required(VERSION 3.15)
project(DSA_String_Compression CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB UTIL_FILES ${CMAKE_SOURCE_DIR}/utils/*.cpp)

add_executable(compressor ${SRC_FILES} ${UTIL_FILES})

target_include_directories(compressor PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/utils
)

if(MSVC)
    target_compile_options(compressor PRIVATE /W4)
else()
    target_compile_options(compressor PRIVATE -Wall -Wextra)
endif()
