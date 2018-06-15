# Testing

For unit tests, external [Catch2](https://github.com/catchorg/Catch2) library will be used.
Just drop its single include header file **catch.hpp** onto *include* project folder.

Due to the fact, **CMake** is used in this project, just typical **ctest** command will do the trick: 

## Windows

Provided you got your latest Windows10 box (>= 1803) with latest Visual Studio Community Edition (>= 15.7.3) and needed tools, as CMake (>= 3.11.3),
usual *CMake* commands, taking for granted they are launched from **Development Command Prompt for VS 2017** console:

	mkdir build
	cd build
	cmake .. -G "Visual Studio 15 2017 Win64"
	cmake --build . --config Release
	ctest -C Release

If you want to launch it directly in order to obtain more details, provided you're in that *build\test\Release* folder:

	cd test\Release
	.\test*.exe --use-colour yes --success


## Linux 

Provided you got your distro up to date for latest c++ compilers and needed tools, as CMake, similar commands to Windows are expected:

	mkdir -p build
	cd build
	cmake .. 
	cmake --build . 
	ctest 

If you want to launch it directly in order to obtain more details, provided you're in that *build/test* folder:

	cd test
	./test* --use-colour yes --success


