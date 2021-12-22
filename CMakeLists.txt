cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
add_definitions("-std=c++11")
add_definitions(-fPIC)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

aux_source_directory(. SRCS)
include_directories(
    ./
    ./include
    ./base
)

find_package(OpenCV 3.1 REQUIRED)

#link_directories()

file(COPY ${PROJECT_SOURCE_DIR}/3rd/zxing/bin DESTINATION ${PROJECT_BINARY_DIR})

add_library(parseQRCode SHARED ${SRCS})

target_link_libraries(parseQRCode  ${OpenCV_LIBS} zxing zbar)

add_subdirectory(tests)

