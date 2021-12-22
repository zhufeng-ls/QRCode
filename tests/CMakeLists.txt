link_directories(/usr/local/lib)

#add_definitions("-fPIC -Wall -g")
add_executable(testParseQR main.cc)
target_link_libraries(testParseQR parseQRCode ${OpenCV_LIBS})

# 拷贝测试图片到当前目录
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${PROJECT_BINARY_DIR})