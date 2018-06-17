/**
* @file test00.cpp 
* @author Andres Sevillano 
* @date June 2018
* @brief Some basic tests on how to work with data structures.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "data_structure.hpp"

using namespace SoftwareChallenge;

SCENARIO("Check how to compact/load data structures", "[structure]") {

    GIVEN("Name & Index maping") {

        NameIndex name2index;

        WHEN("Empty map") {

            name2index.clear();

            THEN("Compact its contain must be just the amount needed for its length") {

                std::vector<uint8_t> compact { name2index.compact() };

                REQUIRE( compact.size() == sizeof(IndexType) );
                IndexType length { *compact.data() };
                REQUIRE( length == 0 );
			}
        }

        WHEN("An empty map is manually loaded") {

            name2index.clear();
            name2index["John"] = 0;
            name2index["Ian"] = 1;

            THEN("Compact its contain must be the amount needed for its length and its payload") {

                std::vector<uint8_t> compact { name2index.compact() };

                IndexType length { *compact.data() };
                REQUIRE( length == 2 );

                NameType* firstName = reinterpret_cast<NameType*>(compact.data() + sizeof(IndexType));
                IndexType firstIndex { *(compact.data() + sizeof(IndexType) + sizeof(NameType)) };

                std::string Ian{reinterpret_cast<char*>(firstName)};
                REQUIRE( Ian == "Ian" );
                REQUIRE( firstIndex == 1 );

                NameType* secondName = reinterpret_cast<NameType*>(compact.data() + 2*sizeof(IndexType) + sizeof(NameType));
                IndexType secondIndex { *(compact.data() + 2*sizeof(IndexType) + 2*sizeof(NameType)) };

                std::string John{reinterpret_cast<char*>(secondName)};
                REQUIRE( John == "John");
                REQUIRE( secondIndex == 0 );
            }

			THEN("It's possible to know whether a member belongs to this social network or not") {

				auto[found, index] = name2index.getIndex("John"); 
				REQUIRE(found == true);
				REQUIRE(index == 0);

				auto[exists, whatever] = name2index.getIndex("Abominable_Snowman");
				REQUIRE(exists == false);
				REQUIRE(whatever == INDEX_MAX);
			}
        }

        WHEN("An empty map is automatically loaded") {

            name2index.clear();
            name2index["John"] = 0;
            name2index["Ian"] = 1;
            std::vector<uint8_t> raw { name2index.compact() };
            name2index.clear();
            IndexType length { name2index.load(raw) };

            THEN("Compact its contain must be the amount needed for its length and its payload") {
               REQUIRE( raw.size() == (3*sizeof(IndexType) + 2*sizeof(NameType)) );
               REQUIRE( length == 2 );
               REQUIRE( name2index.size() == 2);
               REQUIRE( length == name2index.size() );

               IndexType John_index = name2index["John"];
               REQUIRE( John_index == 0 );
               IndexType Ian_index = name2index["Ian"];
               REQUIRE( Ian_index == 1 );
            }
        }
   }
}
