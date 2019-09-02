cmake_minimum_required(VERSION 2.8)

project(field_test)
find_package( OpenCV REQUIRED )
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries( field_test ${OpenCV_LIBS})
