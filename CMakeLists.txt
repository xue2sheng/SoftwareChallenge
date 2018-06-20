# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)
project ("SoftwareChallenge")

# The project version number for binaries and for doxygen
set(VERSION_MAJOR   2    CACHE STRING "Project major version number.")
set(VERSION_MINOR   0    CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0    CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# Include sub-projects.
add_subdirectory ("src")

if(EXTRA)
  # testing with included Catch2 header library
  enable_testing()
  add_subdirectory ("test")

  # Documentation with expected installed Doxygen/Graphviz utitlity
  add_subdirectory("doc")
endif()
