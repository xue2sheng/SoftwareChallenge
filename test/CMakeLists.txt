# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

include_directories(test00 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test00 $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "test00.cpp")

include_directories(test01 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test01 $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "test01.cpp")

include_directories(test02 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test02 $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "test02.cpp")

include_directories(test03 PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable(test03 $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "test03.cpp")

# C++17 required by Catch2 library
set_property(TARGET test00 test01 test02 test03 PROPERTY CXX_STANDARD 17)
set_property(TARGET test00 test01 test02 test03 PROPERTY CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
add_test(NAME test00 COMMAND test00)
add_test(NAME test01 COMMAND test01)
add_test(NAME test02 COMMAND test02)
add_test(NAME test03 COMMAND test03)
