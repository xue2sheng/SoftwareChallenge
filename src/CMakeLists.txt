# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Version info
set(version_in ${CMAKE_CURRENT_SOURCE_DIR}/version.in)
set(version_cpp ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
configure_file(${version_in} ${version_cpp} @ONLY)

# Data path info to avoid differences between Windows (depends on its Debug/Release) and Linux/macOS (that extra level not needed)
set(data_path_in ${CMAKE_CURRENT_SOURCE_DIR}/data_path.in)
set(data_path_cpp ${CMAKE_CURRENT_BINARY_DIR}/data_path.cpp)
configure_file(${data_path_in} ${data_path_cpp} @ONLY)

# pthreads doesn't make Windows happy
SET(CMAKE_CXX_FLAGS -pthread)

# Compile objects just once if possible
include_directories(${CMAKE_PROJECT_NAME}_object PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_library(${CMAKE_PROJECT_NAME}_object OBJECT 
	"commandline.cpp" 
	"preprocess.cpp" 
	"data_structure.cpp" 
	"graph_algorithm.cpp"
	"version.cpp" 
	"data_path.cpp"
)

# Add source to this project's executable.
include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_executable (${CMAKE_PROJECT_NAME} $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "main.cpp")

# C++17 flags
set_property(TARGET ${CMAKE_PROJECT_NAME}_object ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${CMAKE_PROJECT_NAME}_object ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)


# TODO: Add tests and install targets if needed.
