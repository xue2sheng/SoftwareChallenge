# Buiding

Although **CMake** makes it easy to build your code on different platforms, there are always little details to take into account

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
