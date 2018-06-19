/**
* @file test05.cpp 
* @author Andres Sevillano 
* @date June 2018
* @brief Some basic tests on how to work with the generated graph.
*
* Pay attention to the fact that some external defined constant for data paths is used. 
* 
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "graph_algorithm.hpp"
#include "preprocess.hpp"

using namespace SoftwareChallenge;

/*

A0,A1
A1,A2
....
...
broken chain
...
...
A30,A31
...
...

*/
SCENARIO("Process human-friendly inputs", "[file]") {

	GIVEN("A human-friendly input file") {

	/** Generated by CMake in order not to have to face different relative paths on Windows and on Linux  */
	extern const char* SOFTWARE_CHALLENGE_DATA_PATH;
	std::string data_path{ SOFTWARE_CHALLENGE_DATA_PATH };
    data_path += "/test05.txt";

        Collection network;
        auto [ success, hint ] = network.process(data_path);
                REQUIRE( success == true );


	WHEN("Read expected number of members") {

                auto[success, hint, number_of_members, name2index, friendGraph ] = network.compact();
                REQUIRE( number_of_members == 27);

                REQUIRE(name2index["A1"] == 1);
                REQUIRE(name2index["A30"] == 25);
            	

            THEN("Compact information in a computer-friendly way") {

        auto[ success, hint, ties ] = searchFriends("A1","A30", name2index, friendGraph);
                REQUIRE( success == false );
                REQUIRE( ties == INDEX_MAX );
            }

    }

  }
}