# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# pthread doesn't make Windows happy

# pthreads doesn't make Windows happy
if(UNIX OR APPLE)
	set(CMAKE_CXX_FLAGS -pthread)
endif()

foreach(TEST_NAME test00 test01 test02 test03 test04)

	include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
	add_executable(${TEST_NAME} $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}_object> "${TEST_NAME}.cpp")

	# C++17 required by Catch2 library
	set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 17)
	set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

	# cmake target
	add_test(NAME ${TEST_NAME} COMMAND test00)

endforeach(TEST_NAME)

