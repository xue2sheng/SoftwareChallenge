# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)
project ("SoftwareChallenge")

# The project version number for binaries and for doxygen
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   7   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# Include sub-projects.
add_subdirectory ("src")

# testing
enable_testing()
add_subdirectory ("test")

# Documentation
add_subdirectory("doc")
