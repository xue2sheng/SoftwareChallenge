# Software Challenge

Given a **SocialNetwork.txt** input file with the following structure:

    MYLES_JEFFCOAT,LANNY_TIBURCIO
    MYLES_JEFFCOAT,MARTIN_OMERSA
    MYLES_JEFFCOAT,SAM_SWAIT
    MYLES_JEFFCOAT,TERENCE_SEISLER

The file represents an imaginary social network of people. Each line in the file provides the
names of two people who are friends. Write a program that

1. determines the total number of people in the social network

2. determines the distance between A and B where the distance between two members of
the network can be defined as the minimum number of ties required to connect two
people in the social network

For this challenge, the values of A and B are as follows: A = STACEY_STRIMPLE, B =
RICH_OMLI

## Simplifications

Some facts will be supposed in order to simplify the solution:

1. The input file is not too huge and it can be processed as a whole in the available memory. 
For example, 15 Mb of pre-processed text input against 32 Gb of available memory.

2. Names are encoded in typical ASCII format and they are not too lengthy. 
Please, no more *emojis* this time ;)

3. The input doesn't change too often so it makes sense to invest time and resources on pre-processing once 
to get a more *computer-friendly* equivalent. As well this pre-process lets us collect stats on input data, 
always useful to tune code later on.

## Approach

Typical divide & conquer approach:

1. One binary for the **pre-process** of the original input file and 
provides *at least* the total number of people in the social network 
and more *computer-friendly* inputs for following steps.

2. One binary that reads that previously processed input and determines 
the **distance** between A and B as the minimum number of ties required 
to connect A and B.

![divide and conquer](images/divide_and_conquer.png)

That way it's easier to test and to hunt bugs down, 
taking incremental steps as the ones in typical Agile sprints. 
Besides, if some **microserver** solution might be tried, getting that task division
will help out a lot.

## Working environment

Part of the challenge, from point of view of a Linux user like me, is to have as primary working environent a Windows box.
*Microsoft Visual Studio Community Edition* with support for **C++/WinRT** was the main IDE used.
Furthermore, **commandline** alternatives are always provided because *CMake* was heavily used & abused.

Anyway, Linux tips are usually provided and going from there to macOS. Well, using QtCreator/CMake projects could be a 
little easier than go directly for XCode on macOS.

### From your favourite IDE

Depending on your choice, i.e. Visual Studio or QtCreator, you'll need to go for a **CMake** project type.
Then configure your options to kick off only the proper binaries instead of their default options.

For example, in Visual Studio you should choose the proper architecture (x64) and version (Release).
Meanwhile with QtCreator you should choose your proper building directory.

In any case, don't forget to add to that **.gitignore** their specific utility files and don't hesitate to load convenient plugins to support *vim bindings*, *markdown files* or *spell correction*.
  
### From commandline

**Do not skip** the ability of working from commandline in order to **make it easier** future *DevOps* tasks to get your team's work flow properly automated.

In this case, being just a *toy project*,  usual **CMake** commands should suffice, taking into account you'll be working from your *build* folder.

#### Build & launch 
See [Building hints](BUILDING.md)

#### Tests
See [Testing hints](TESTING.md)

#### Documentation
See [Documentation hints](DOCUMENTATION.md)