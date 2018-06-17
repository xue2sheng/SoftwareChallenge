# Buiding

Although [**CMake**](CMakeLists.txt) makes it easy to build your code on different platforms, there are always little details to take into account. 

## Windows

Provided you got your latest Windows10 box (>= 1803) with latest Visual Studio Community Edition (>= 15.7.3) and needed tools, as CMake (>= 3.11.3),
usual *CMake* commands, taking for granted they are launched from **Development Command Prompt for VS 2017** console:

	mkdir build
	cd build
	cmake .. -G "Visual Studio 15 2017 Win64"
	cmake --build . --config Release

Double check that binary was generated without strange dependencies with **dumpbin** (similar to **ldd** on Linux):

	dumpbin .\src\Release\*.exe

Launching that binary from that **build** directory:

	.\src\Release\*.exe

## Linux

Provided you got your distro up to date for latest gcc c++ compilers and needed tools, as CMake, similar commands to Windows are expected:

        mkdir -p build
        cd build
        cmake ..
        cmake --build .

Double check that binary was generated without strange dependencies with **ldd**:

	ldd `file ./src/* | grep ELF | sed "s/^\(.*\):.*$/\1/g"`

Launching that binary from that **build** directory:
	
	echo `file ./src/* | grep ELF | sed "s/^\(.*\):.*$/\1/g"` | bash

## macOS

Provided you got update your macOS box with latest apple clang c++ compiler (>= 9.1.0) and needed tools, as CMake (>= 3.11.4), maybe through Homebrew or similar, almost the very same Linux commands:

	mkdir -p build
	cd build
	cmake ..
	cmake --build .

Double check that binary was generated without strange dependencies with **otool**:

	otool -L `file src/* | grep executable | sed "s/^\(.*\):.*$/\1/g"`

Launchng that binary from that **build** directory:

	echo `file src/* | grep executable | sed "s/^\(.*\):.*$/\1/g"` | bash

# Version number

Being a *toy project*, that typical version number needed to keep in sync documentation and binaries versions has been directly defined at the main CMakeLists.txt file instead of the more usual version.txt managed by extra scripts or extra cmake instructions.

	# CMakeList.txt : Top-level CMake project file, do global configuration
	# and include sub-projects here.
	#
	cmake_minimum_required (VERSION 3.10)
	project ("SoftwareChallenge")
	#
	# The project version number for binaries and for doxygen
	set(VERSION_MAJOR   0    CACHE STRING "Project major version number.")
	set(VERSION_MINOR   0    CACHE STRING "Project minor version number.")
	set(VERSION_PATCH   10   CACHE STRING "Project patch version number.")
	mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
	#
	# Include sub-projects.
	add_subdirectory ("src")
	#
	if(EXTRA)
	  #
	  # testing with included Catch2 header library
	  enable_testing()
	  add_subdirectory ("test")
	  #
	  # Documentation with expected installed Doxygen/Graphviz utitlity
  	  add_subdirectory("doc")
	  #
	endif()

That means that you have to drop CMake caches to really update that number in your executables or in your *doxygen* generated documentation; removing the whole build directory could be the most effective path. Don't forget that detail, specially if you work with IDE's.
