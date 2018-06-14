# Working environment

Part of the challenge, from point of view of a Linux user like me, is to have as primary working environent a Windows box.
*Microsoft Visual Studio Community Edition* with support for **C++/WinRT** was the main IDE used.
Furthermore, **commandline** alternatives are always provided because *CMake* was heavily used & abused.

Anyway, Linux tips are usually provided and going from there to macOS. Well, using QtCreator/CMake projects could be a 
little easier than go directly for XCode on macOS.

## From your favourite IDE

Depending on your choice, i.e. Visual Studio or QtCreator, you'll need to go for a **CMake** project type.
Then configure your options to kick off only the proper binaries instead of their default options.

For example, in Visual Studio you should choose the proper architecture (x64) and version (Release).
Meanwhile with QtCreator you should choose your proper building directory.

In any case, don't forget to add to that **.gitignore** their specific utility files and don't hesitate to load convenient plugins to support *vim bindings*, *markdown files* or *spell correction*.
  
## From commandline

**Do not skip** the ability of working from commandline in order to **make it easier** future *DevOps* tasks to get your team's work flow properly automated.

In this case, being just a *toy project*,  usual **CMake** commands should suffice, taking into account you'll be working from your *build* folder.

### Build & launch 
See [Building hints](BUILDING.md)

### Tests
See [Testing hints](TESTING.md)

### Documentation
See [Documentation hints](DOCUMENTATION.md)