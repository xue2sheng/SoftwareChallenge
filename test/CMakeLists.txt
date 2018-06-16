# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

add_executable(test00 "test00.cpp")

include_directories(test01 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test01 $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "test01.cpp")

# C++17 required by Catch2 library
set_property(TARGET test00 test01 PROPERTY CXX_STANDARD 17)
set_property(TARGET test00 test01 PROPERTY CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
add_test(NAME test00 COMMAND test00)
add_test(NAME test01 COMMAND test01)