/**
* @file main.cpp 
* @author Andres Sevillano 
* @date June 2018
* @brief Silly process to check it out ideas before writing down proper unit tests.
*
* If you launch this program from its building folder, the default values should be good enough:
*
*    src/SoftwareChallenge
*
* Anyway, have a lookk to its help as usual:
*
*    src/SoftwareChallenge --help
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <iostream>
#include "preprocess.hpp"
#include "graph_algorithm.hpp"

int main(int argc, char** argv)
{
	// preprocess human-friendly input into something more compact
    auto[success, hint, stats, number_of_members, coupleList, name2index, friendGraph] = preprocess(argc, argv);
	if (!success) {
		std::cerr << "ERROR: " << hint.c_str() << std::endl; 
        return -1;
	} 

	// minimum info
	std::cout << std::endl << "Number of members in that social network: " << number_of_members << std::endl << std::endl;

	// some extra stats if required
	if(stats) { std::cout << hint.c_str() << std::endl; }

    // success --> 0
    int result {0};

    // search for ties
    for(const auto& i : coupleList) {

        auto [success, hint, ties] = searchFriends( i.first, i.second, name2index, friendGraph );
        if( !success ) {
            std::cerr << "ERROR: " << hint.c_str() << std::endl;
            ++result; // means no success in the end
            continue; // process all searches
        }
        std::cout << hint.c_str() << std::endl;
    }

    // exit with proper code ==> good for possible external scripts by our DevOps fellas
    return result;
}

// src/SoftwareChallenge -s -g -f ../data/SocialNetwork.txt ../data/SocialNetwork.bin
// Number of members in that social network: 82168
// size=82168 relationships=1008460 name_min=6 name_max=24 popular_min=AARON_BOUSMAN friends_min=1 popular_max=CLIFTON_TIMMIS friends_max=2552 stored in file /home/user/Code/SoftwareChallenge/build/SocialNetwork.bin
