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

Windows 10 with Visual Studio Community Edition was the default one but as well it was
checked out on Linux/macOS boxes when possible. See [Working environment hints](ENVIRONMENT.md)