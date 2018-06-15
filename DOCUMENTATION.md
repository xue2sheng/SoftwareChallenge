# Documentation

Just using the usual suspect [Doxygen](www.stack.nl/~dimitri/doxygen/download.html
). See [further details](https://p5r.uk/blog/2014/cmake-doxygen.html) on how to configure your environment
but **don't** forget to have [Graphviz](https://graphviz.gitlab.io/_pages/Download/Download_windows.html) installed in order to make **dot** available.

So comment properly your code and invoke the usual suspect **CMake** target. 
Pay attention to the fact that building your code is not needed for generation your documentation.

## Windows

Provided you got your latest Windows10 box (>= 1803) with latest Visual Studio Community Edition (>= 15.7.3) and needed tools, as CMake (>= 3.11.3) or Doxygen (>= 1.8.14),
usual *CMake* commands, taking for granted they are launched from **Development Command Prompt for VS 2017** console:

	mkdir build
	cd build 
	cmake .. -G "Visual Studio 15 2017 Win64" -DBUILD_DOCUMENTATION:STRING=yes
	doxygen doc/Doxyfile

The documentation should be generated inside of **html** folder at that *build* directory:

	start firefox .\html\index.html

## Linux 

Provided you got your latest needed tools, as CMake or Doxygen/Graphviz, on your distro, usual commands are expected:

	mkdir -p build
	cd build 
	cmake ..
	doxygen doc/Doxyfile

The documentation should be generated inside of **html** folder at that *build* directory:

	firefox .\html\index.html

# Diagrams

Among zillions of available possibilities, [PlantUML](plantuml.com) lets you generate **dot** based diagrams from text files.
Besides, it could be integrated with *Doxygen* and those diagrams keep their meta-information embedded. 
So if needed in the future, from the very same image, you can obtain and update manually its contents.

In case of this *toy project*, no extra *CMake* targets will be added to generate those images or
to double check its embedded meta-information to avoid regenerate them if not needed (that way
you don't end uploading again and again your images to your repository).

Provided your diagram is saved as *diagram.uml* and **plantuml.jar** is placed at *C:/plantuml/plantuml.jar* on Windows, generate and check with:

	cd .\images
	java -jar C:\plantuml\plantuml.jar -progress .\diagram.uml
	start firefox .\diagram.png

On Linux, something similar provided that *jar* file is placed at */opt/plantuml/plantuml.jar* instead:

	cd ./images	
	java -jar /opt/plantuml/plantuml.jar -progress ./diagram.uml
	firefox ./diagram.png
