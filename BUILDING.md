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

For exemple, on some Windows 10:

	Microsoft (R) COFF/PE Dumper Version 14.14.26430.0
	Copyright (C) Microsoft Corporation.  All rights reserved.
	Dump of file .\build\src\Release\SoftwareChallenge.exe
	File Type: EXECUTABLE IMAGE
	  Summary
		1000 .data
		2000 .pdata
		8000 .rdata
		1000 .reloc
		1000 .rsrc
	       14000 .text

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

For example, in one ArchLinux box:

	linux-vdso.so.1 (0x00007ffd0cde4000)
	libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007f3acf9a3000)
	libm.so.6 => /usr/lib/libm.so.6 (0x00007f3acf60e000)
	libgcc_s.so.1 => /usr/lib/libgcc_s.so.1 (0x00007f3acf3f6000)
	libpthread.so.0 => /usr/lib/libpthread.so.0 (0x00007f3acf1d8000)
	libc.so.6 => /usr/lib/libc.so.6 (0x00007f3acee1c000)
	/lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007f3acff67000)

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

For example, in one High Sierra box:

	src/SoftwareChallenge:
		/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 400.9.0)
		/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.50.4)

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

# Sanity Checks

Besides the ones included in the common IDE's, i.e. Visual Studio with Microsoft Code Analysis, it's good to have a look to external sanity checker in a real project. This one is just a *toy project* so **valgrind** should be enough:

 
	==5682== Memcheck, a memory error detector
	==5682== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
	==5682== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
	==5682== Command: build/src/SoftwareChallenge
	==5682== Parent PID: 5241
	==5682== 
	==5682== 
	==5682== HEAP SUMMARY:
	==5682==     in use at exit: 0 bytes in 0 blocks
	==5682==   total heap usage: 2,022,813 allocs, 2,022,813 frees, 91,959,214 bytes allocated
	==5682== 
	==5682== All heap blocks were freed -- no leaks are possible
	==5682== 
	==5682== For counts of detected and suppressed errors, rerun with: -v
	==5682== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

# Pending tasks

Due to the fact that *valgrind* instrumentation slows down that default execution, it's clear that getting the graph ready takes a good chunk of the total time execution. It'd be great to load directly some preprocessed computer-friendly input instead.

That the reason why one attempt to code **binary** input files was tried but not completely unit testes.
So take it that part as a pending **proof of concept** but don't execute. It seems that the size of the binary version
can be almost the half because *strings* are avoided as much as possible.

As well the minimum possible size was chosen but it can lead to issues when it comes down to load binary information from the hard disk. Ditching that usual type **size_t**, expected by common low level operations was a handicap. 
But at least it showed that with some extra time to iron out all the currently pending problems on that subject, not only hard disk space can be spared but also loading time. 

**Preprocess** the input as a different task from the very same **search for the target** can be considered as a first step in order to split this monolithic app into several more **microservices**.
In the code, it can be found to different *functions*: **preprocess** and **searchFriends**. See [The Twelve=factor App](https://12factor.net/)

