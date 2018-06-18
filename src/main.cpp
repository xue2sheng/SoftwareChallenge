﻿/**
* @file main.cpp 
* @author Andres Sevillano 
* @date June 2018
* @brief Silly process to check it out ideas before writing down proper unit tests.
*
* Pay attention to the fact that some external defined constant for the  version number is used. 
* 
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <iostream>
#include "preprocess.hpp"

int main(int argc, char** argv)
{
	// preprocess human-friendly input into something more compact
    auto[success, hint, stats, number_of_members, name2index, friendGraph] = preprocess(argc, argv);
	if (!success) {
		std::cerr << "ERROR: " << hint.c_str() << std::endl; 
		return 1;
	} 

	// minimum info
	std::cout << std::endl << "Number of members in that social network: " << number_of_members << std::endl << std::endl;

	// some extra stats if required
	if(stats) { std::cout << hint.c_str() << std::endl; }

	// success
	return 0;
}


