# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Data path info
set(data_path_in ${CMAKE_CURRENT_SOURCE_DIR}/data_path.in)
set(data_path_cpp ${CMAKE_CURRENT_BINARY_DIR}/data_path.cpp)
configure_file(${data_path_in} ${data_path_cpp} @ONLY)

# Add source to this project's executable.
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_SOURCE_DIR}/include)

#add_executable(test00 ${CMAKE_CURRENT_SOURCE_DIR}/test00.cpp)
#target_link_libraries(test00 Catch)
#set_property(TARGET test00 PROPERTY CXX_STANDARD 17)
#set_property(TARGET test00 PROPERTY CXX_STANDARD_REQUIRED ON)

include_directories(test01 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test01 ${CMAKE_CURRENT_SOURCE_DIR}/test01.cpp ${CMAKE_CURRENT_BINARY_DIR}/data_path.cpp ${CMAKE_SOURCE_DIR}/src/file.cpp)
target_link_libraries(test01 Catch)
set_property(TARGET test01 PROPERTY CXX_STANDARD 17)
set_property(TARGET test01 PROPERTY CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
#add_test(NAME test00 COMMAND test00)
add_test(NAME test01 COMMAND test01)