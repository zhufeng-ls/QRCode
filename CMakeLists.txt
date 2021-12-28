cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
add_definitions("-std=c++11")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -std=c++11")
add_definitions(-fPIC)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

aux_source_directory(./src SRCS)
include_directories(
    ./
    ./include
    ./base
    ./3rd/zxing/include
    ./3rd/zbar/include
)

find_package(OpenCV 3.1 REQUIRED)

#link_directories()
link_directories(${PROJECT_BINARY_DIR}/lib)

file(COPY ${PROJECT_SOURCE_DIR}/3rd/zxing/bin/zxing DESTINATION ${PROJECT_BINARY_DIR}/bin)

file(COPY ${PROJECT_SOURCE_DIR}/3rd/zxing/lib/libzxing.a DESTINATION ${PROJECT_BINARY_DIR}/lib)
file(COPY ${PROJECT_SOURCE_DIR}/3rd/zbar/lib/libzbar.so.0 DESTINATION ${PROJECT_BINARY_DIR}/lib)
file(COPY ${PROJECT_SOURCE_DIR}/3rd/zbar/lib/libzbar.so DESTINATION ${PROJECT_BINARY_DIR}/lib)

add_library(parseQRCode SHARED ${SRCS})

target_link_libraries(parseQRCode  ${OpenCV_LIBS} zxing zbar)

add_subdirectory(tests)

