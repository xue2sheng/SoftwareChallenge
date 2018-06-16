/**
* @file data_structure.cpp
* @author Andres Sevillano
* @date June 2018
* @brief Utilities to compact computer-friendly collection info.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/
/** @brief Helper for options */

#include <iostream>
#include "commandline.hpp"

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return nullptr;
}

/** @brief Helper for options */
bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

std::tuple<std::string, bool> commandline_arguments(int argc, char** argv)
{
	if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help")) {
		std::cout << std::endl << "Software Challenge" << std::endl << std::endl;

		std::cout << " -h --help: This help information." << std::endl;
		std::cout << " -v --version: Binary version to match Doxygen configuration one." << std::endl;
		std::cout << " -f <file_name>: Input file name. If not provided, a default one will be used." << std::endl;
		std::cout << std::endl;

		exit(0); // nothing more to do
	}

	if (cmdOptionExists(argv, argv + argc, "-v") || cmdOptionExists(argv, argv + argc, "--version")) {

		// Generated by CMake in order not to have to face different relative paths on Windows and on Linux  
		extern const char* SOFTWARE_CHALLENGE_VERSION;
		std::cout << std::endl << SOFTWARE_CHALLENGE_VERSION << std::endl << std::endl;
		exit(0); // nothing more to do
	}

	// stats on input file
	bool stats{ cmdOptionExists(argv, argv + argc, "-s") || cmdOptionExists(argv, argv + argc, "--stats") };

	std::string file_name{ "" };
	if (char* ptr = getCmdOption(argv, argv + argc, "-f"); ptr != nullptr) {

		file_name = std::string{ ptr }; // a copy just in case argv[] got missing in action

	} else { // default value

		// Generated by CMake in order not to have to face different relative paths on Windows and on Linux  
		extern const char* SOFTWARE_CHALLENGE_DATA_PATH;
		std::string data_path{ SOFTWARE_CHALLENGE_DATA_PATH };

		// size=402 name_min=6 name_max=20 popular_min=ABEL_BONNES friends_min=1 popular_max=MYLES_JEFFCOAT friends_max=215
		// file_name = data_path + "/test01.txt";

		// size=82168 name_min=6 name_max=24 popular_min=AARON_BOUSMAN friends_min=1 popular_max=CLIFTON_TIMMIS friends_max=2552
		file_name = data_path + "/SocialNetwork.txt";
	}

	return { file_name, stats };
}