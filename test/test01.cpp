/**
* @file test01.cpp 
* @author Andres Sevillano 
* @date June 2018
* @brief Some basic tests on how to process input file.
*
* Pay attention to the fact that some external defined constant for data paths is used. 
* 
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <cstring>
#include "preprocess.hpp"

#include <iostream>

using namespace SoftwareChallenge;

SCENARIO("Process human-friendly inputs", "[file]") {

	GIVEN("A human-friendly input file") {

		Collection network{};
		REQUIRE(network.size() == 0);

		/** Generated by CMake in order not to have to face different relative paths on Windows and on Linux  */
		extern const char* SOFTWARE_CHALLENGE_DATA_PATH;
		std::string data_path{ SOFTWARE_CHALLENGE_DATA_PATH };
        data_path += "/test01.txt";

		auto [ success, hint ] = network.process(data_path);
		REQUIRE( success == true );
		REQUIRE( hint == "size=402 name_min=6 name_max=20" );

		WHEN("Read expected number of members") {

			//MYLES_JEFFCOAT, LANNY_TIBURCIO
			auto Lanny = network["LANNY_TIBURCIO"];
			REQUIRE(Lanny.size() > 0);
			auto Nyles = network["MYLES_JEFFCOAT"];
			REQUIRE(Nyles.size() > 0);

			THEN("MYLES_JEFFCOAT and LANNY_TIBURCIO should be friends") {
				REQUIRE( Nyles.end() != Nyles.find(Lanny.index) );
			}
			THEN("MARIANO_UMPHRESS should get some index") {
				auto Mariano = network["MARIANO_UMPHRESS"];
				REQUIRE(Mariano.size() > 0);
			}
			THEN("NICKY_MURR should get the latest index") {
				auto Nicky = network["NICKY_MURR"];
				REQUIRE(Nicky.size() > 0);
				REQUIRE(Nicky.index == (network.size() - 1));
			}

			THEN("Compact information in a computer-friendly way") {
                auto[success, hint, number_of_members, name2index, friendGraph ] = network.compact();
		REQUIRE( success == true );
		REQUIRE(number_of_members == 402);
                REQUIRE( hint == "size=402 relationships=866 name_min=6 name_max=20 popular_min=ABEL_BONNES friends_min=1 popular_max=MYLES_JEFFCOAT friends_max=215" );

                // name->index OK
		REQUIRE(name2index.size() == 402);
		auto[exists, index] = name2index.getIndex("NICKY_MURR");
		REQUIRE(exists == true);
                REQUIRE(index == 401);

                // friend graph OK
                // size=402 name_min=6 name_max=20
                REQUIRE(friendGraph.size() == 402);
                // popular_min=ABEL_BONNES friends_min=1
                auto[foundAbel, indexAbel] = name2index.getIndex("ABEL_BONNES");
                REQUIRE(foundAbel == true);
                REQUIRE(friendGraph[indexAbel].size() == 1);
                REQUIRE(friendGraph[indexAbel].capacity() == 1);
                // popular_max=MYLES_JEFFCOAT friends_max=215
                auto[foundMyles, indexMyles] = name2index.getIndex("MYLES_JEFFCOAT");
                REQUIRE(foundMyles == true);
                REQUIRE(friendGraph[indexMyles].size() == 215);
                REQUIRE(friendGraph[indexMyles].capacity() == 215);
            }
        }

        WHEN("Store its compact format version") {

            extern const char* SOFTWARE_CHALLENGE_BUILD_DIR;
            std::string binary_path{ SOFTWARE_CHALLENGE_BUILD_DIR };
            binary_path += "/test01.bin";

            auto [stored, issues, length, name2index_length, friendGraph_length ] = network.store(binary_path);
            REQUIRE( length == 402 );
            REQUIRE( name2index_length == 14476 ); // for small test01.txt size(NameIndex) > size(friendGraph)
            REQUIRE( friendGraph_length == 8292 );

            //MYLES_JEFFCOAT --> 0, LANNY_TIBURCIO --> 1
            auto[foundMyles, indexMyles] = network.name2index.getIndex("MYLES_JEFFCOAT");
            REQUIRE(foundMyles == true);
            REQUIRE(indexMyles == 0);
            auto[foundLanny, indexLanny] = network.name2index.getIndex("LANNY_TIBURCIO");
            REQUIRE(foundLanny == true);
            REQUIRE(indexLanny == 1);

            THEN("Able to store binary format") {

               REQUIRE( issues == "Stored compact format at " + binary_path );
               REQUIRE( stored == true );
            }

            THEN("Able to load that very same binary format") {

               REQUIRE( issues == "Stored compact format at " + binary_path );
               REQUIRE( stored == true );

               network.reset();
               auto[ loaded, problems, num_of_members, name2index_size, friendGraph_size ] = network.load(binary_path);
               REQUIRE( num_of_members == length );
               REQUIRE( name2index_length == name2index_size ); // for small test01.txt size(NameIndex) > size(friendGraph)
               REQUIRE( friendGraph_length == friendGraph_size );
               REQUIRE( problems == "Loaded binary data from " + binary_path );
               REQUIRE( loaded == true );

               //MYLES_JEFFCOAT--> 0, LANNY_TIBURCIO -=> 1
               auto[foundMyles, indexMyles] = network.name2index.getIndex("MYLES_JEFFCOAT");
               REQUIRE(foundMyles == true);
               REQUIRE(indexMyles == 0);
               auto[foundLanny, indexLanny] = network.name2index.getIndex("LANNY_TIBURCIO");
               REQUIRE(foundLanny == true);
               REQUIRE(indexLanny == 1);
            }
        }
   }
}
