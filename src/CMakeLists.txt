# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Version info
set(version_in ${CMAKE_CURRENT_SOURCE_DIR}/version.in)
set(version_cpp ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
configure_file(${version_in} ${version_cpp} @ONLY)

# Add source to this project's executable.
include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable (${CMAKE_PROJECT_NAME} "main.cpp" "file.cpp" "version.cpp")
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# TODO: Add tests and install targets if needed.
