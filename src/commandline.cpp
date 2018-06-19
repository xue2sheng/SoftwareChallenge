/**
* @file commandline.cpp
* @author Andres Sevillano
* @date June 2018
* @brief Utilities to compact computer-friendly collection info.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/
/** @brief Helper for options */

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>
#include <cctype>
#include "commandline.hpp"

/**
* @brief paif of member to process
*/
typedef std::vector<std::pair<std::string, std::string>> CoupleList;

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

/** @brief Only based on its extension. No further checks (as 'file' linux command) */
bool isBinary(const std::string& file_name)
{
   return (file_name.substr(file_name.find_last_of(".") + 1) == "bin");
}

/** @brief Only based on its extension. No further checks (as 'file' linux command) */
bool isText(const std::string& file_name)
{
   return (file_name.substr(file_name.find_last_of(".") + 1) == "txt");
}

std::tuple< std::string, bool, std::string, bool, bool, CoupleList> commandline_arguments(int argc, char** argv)
{
	if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help")) {
		std::cout << std::endl << "Software Challenge" << std::endl << std::endl;

		std::cout << " -h --help: This help information." << std::endl;
        std::cout << " -v --version: Binary version to match Doxygen configuration one." << std::endl;
        std::cout << " -d --default: Default paths and files." << std::endl;
        std::cout << " -s --stats: Some extra stats on human-friendly input file." << std::endl;
        std::cout << " -f <file_name>: Input file name. If not provided, a default one will be used." << std::endl;
        std::cout << "                 Requires an extension .txt for text files and .bin for binaries." << std::endl;
        std::cout << " -c <file_name>: Compact file name. If not provided, a defult one will be used ." << std::endl;
        std::cout << " -g --generate: Generate computer-friendly compact file name." << std::endl;
        std::cout << "                Required that input file name extension is .txt" << std::endl;
        std::cout << "                This generation option prevents the graph search for number of ties." << std::endl;
        std::cout << " -l <list of name pairs>: list of couples of members to process separated by commas." << std::endl;
        std::cout << "                          There must be even and -g option shouldn't be active." << std::endl;
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

    // Generated by CMake in order not to have to face different relative paths on Windows and on Linux
    extern const char* SOFTWARE_CHALLENGE_DATA_PATH;
    std::string file_name{ "" };

	if (char* ptr = getCmdOption(argv, argv + argc, "-f"); ptr != nullptr) {

		file_name = std::string{ ptr }; // a copy just in case argv[] got missing in action

	} else { // default value

		// Microsoft Code Analysis might trigger a false warning about previous char* missing ending '\0': ignore it
		const std::string data_path{ SOFTWARE_CHALLENGE_DATA_PATH };

        // size=402 relationships=866 name_min=6 name_max=20 popular_min=ABEL_BONNES friends_min=1 popular_max=MYLES_JEFFCOAT friends_max=215
        // file_name = data_path + "/test01.txt";
        // file_name = data_path + "/test01.bin";

        // size=82168 relationships=1008460 name_min=6 name_max=24 popular_min=AARON_BOUSMAN friends_min=1 popular_max=CLIFTON_TIMMIS friends_max=2552
        //file_name = data_path + "/SocialNetwork.txt";
        file_name = data_path + "/SocialNetwork.bin";
    }

    // is the input already in binary mode?
    // Then no need to preprocees anything
    bool binary{ isBinary(file_name) };

    if( ! binary && ! isText(file_name) ) {
       std::cout << std::endl << "The input file '" + file_name + "' doesn't end in '.bin' or '.txt'. Abort execution." << std::endl << std::endl;
       exit(1);
    }

    // generate computer-friendly compact file
    bool generate{ cmdOptionExists(argv, argv + argc, "-g") || cmdOptionExists(argv, argv + argc, "--generate") };

    // Generated by CMake in order not to have to face different relative paths on Windows and on Linux, specially on /tmp
    extern const char* SOFTWARE_CHALLENGE_BUILD_DIR;
    std::string compact_file_name{ "" };

    if (char* ptr = getCmdOption(argv, argv + argc, "-c"); ptr != nullptr) {

        compact_file_name = std::string{ ptr }; // a copy just in case argv[] got missing in action

    } else { // default value

        // Microsoft Code Analysis might trigger a false warning about previous char* missing ending '\0': ignore it
        const std::string binary_path{ SOFTWARE_CHALLENGE_BUILD_DIR };

        // size=402 relationships=866 name_min=6 name_max=20 popular_min=ABEL_BONNES friends_min=1 popular_max=MYLES_JEFFCOAT friends_max=215
        // compact_file_name = binary_path + "/test01.bin";

        // size=82168 relationships=1008460 name_min=6 name_max=24 popular_min=AARON_BOUSMAN friends_min=1 popular_max=CLIFTON_TIMMIS friends_max=2552
        compact_file_name = binary_path + "/SocialNetwork.bin";
    }

    if (cmdOptionExists(argv, argv + argc, "-d") || cmdOptionExists(argv, argv + argc, "--default")) {

        std::cout << std::endl << "Default build directory: " << SOFTWARE_CHALLENGE_BUILD_DIR << std::endl;
        std::cout << "Default data path: " << SOFTWARE_CHALLENGE_DATA_PATH << std::endl << std::endl;
        std::cout << "Default input file path " << file_name << std::endl;
        std::cout << "Default compact file path " << compact_file_name << std::endl << std::endl;
        std::cout << "Default pair of friends to process STACEY_STRIMPLE and RICH_OMLI" << std::endl << std::endl;
        exit(0); // nothing more to do
    }

    CoupleList coupleList{};
    if( ! generate ) {

        if (char* ptr = getCmdOption(argv, argv + argc, "-l"); ptr != nullptr) {

        // some checks
        std::string line(ptr);
        if (line.empty()) {
            std::cerr << "Empty list of members to process" << std::endl << std::endl;
            exit(1);
        }

        // remove non alphanumeric characters, except of "_" and ","
        line.erase(
            std::remove_if(
            line.begin(),
            line.end(),
            [](char c) { return !(std::isalnum(c) || c == '_' || c == ',');  }
            ),
            line.end()
        );

        // again check if something remains
        if (line.size() < 3) {
            std::cerr << "Missing one valid at least couple of members to look for." << std::endl << std::endl;
            exit(1);
        }

        // Out of laziness, replace ',' to split that string
        std::replace(line.begin(), line.end(), ',', ' ');

        std::istringstream iss(line);
        std::vector<std::string> nameList((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

        if( nameList.empty() ) {
            std::cerr << "Empty list of pair of members to process." << std::endl << std::endl;
            exit(1);
        }


        size_t number_of_couples { nameList.size() / 2 };
        if( nameList.size() % 2 != 0 ) {
            std::cerr << "Needed a number even of names to be processed by pairs." << std::endl << std::endl;
            exit(1);
        }

        for(auto i = 0, j = 0; i < number_of_couples; ++i) {
            const auto& A { nameList[j++] };
            const auto& B { nameList[j++] };
            std::cout << " Search " << i << ": " << A << " and " << B << std::endl;
            coupleList.emplace_back( A, B );
        }

        } else { // default value

        std::cout << std::endl << "Searching for default possible friends A = STACEY_STRIMPLE, B = RICH_OMLI" << std::endl;
        coupleList.emplace_back( "STACEY_STRIMPLE", "RICH_OMLI" );
        }

    } // if generate

    return { file_name, binary, compact_file_name, stats, generate, coupleList };
}
