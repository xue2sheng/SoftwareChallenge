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

/** @brief Kepp an eye on data structures **/
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

   GIVEN("Friendship graph structure") {

       FriendGraph friendGraph{};

       WHEN("Empty array") {

            friendGraph.clear();

            THEN("Compact its contain must be just the amount needed for its length") {

                std::vector<uint8_t> compact { friendGraph.compact() };

                REQUIRE( compact.size() == sizeof(IndexType) );
                IndexType length { *compact.data() };
                REQUIRE( length == 0 );
            }
       }

       WHEN("An empty array is manually loaded") {

            friendGraph.clear();
            friendGraph.emplace_back(std::vector<IndexType>{2,1});  // 0 -> 2,1 (popularty order)
            friendGraph[0].resize(2);
            REQUIRE(friendGraph[0].size() == 2);

            friendGraph.emplace_back(std::vector<IndexType>{0});	// 1 -> 0
            friendGraph[1].resize(1);
            REQUIRE(friendGraph[1].size() == 1);

            friendGraph.emplace_back(std::vector<IndexType>{0,3});	// 2 -> 0,3 (popularity order)
            friendGraph[1].resize(2);
            REQUIRE(friendGraph[2].size() == 2);

            friendGraph.emplace_back(std::vector<IndexType>{2}); 	// 3 -> 2
            friendGraph[1].resize(1);
            REQUIRE(friendGraph[3].size() == 1);

            size_t relations {0};
            for(const auto& i : friendGraph ) { relations += i.size(); }
            REQUIRE( relations == 6 );

            THEN("Compact its contain must be the amount needed for its length and its payload") {

                std::vector<uint8_t> compact { friendGraph.compact() };
                REQUIRE( compact.size() == friendGraph.neededBytes() );
                REQUIRE(
                     friendGraph.neededBytes() ==
                     ( sizeof(IndexType) + (4 * (sizeof(size_t) + sizeof(IndexType))) + (6 * (sizeof(IndexType))) )
                );

                // size
                IndexType length { *compact.data() };
                REQUIRE( length == 4 );

                // offsets --> IndexType==4 bytes, size_t==8 bytes
                // pointing to friends of 0
                REQUIRE( static_cast<size_t>(*(compact.data() + 4 + 0*8)) == (4 + 4*8) );
                // pointing to friends of 1
                REQUIRE( static_cast<size_t>(*(compact.data() + 4 + 1*8)) == (4 + 4*8 + 3*4) );
                // pointing to friends of 2
                REQUIRE( static_cast<size_t>(*(compact.data() + 4 + 2*8)) == (4 + 4*8 + 3*4 + 2*4) );
                // pointing to friends of 3
                REQUIRE( static_cast<size_t>(*(compact.data() + 4 + 3*8)) == (4 + 4*8 + 3*4 + 2*4 + 3*4) );

                // number of friends
                // pointing to friends of 0 size=2
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8) ) == 2 );
                // pointing to friends of 1 size=1
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4) ) == 1 );
                // pointing to friends of 2 size=2
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 2*4) ) == 2 );
                // pointing to friends of 3 size=1
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 2*4 + 3*4) ) == 1 );

                // friends
                // pointing to friends of 0 -> 2,1
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 4 + 0*4) ) == 2 );
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 4 + 1*4) ) == 1 );
                // pointing to friends of 1 -> 0
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 4 + 0*4) ) == 0 );
                // pointing to friends of 2 -> 0,3
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 2*4 + 4 + 0*4) ) == 0 );
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 2*4 + 4 + 1*4) ) == 3 );
                // pointing to friends of 3 -> 2
                REQUIRE( static_cast<IndexType>(*(compact.data() + 4 + 4*8 + 3*4 + 2*4 + 3*4 + 4 + 0*4) ) == 2 );
            }
        }

        WHEN("An empty array is automatically loaded") {

            friendGraph.clear();
            friendGraph.emplace_back(std::vector<IndexType>{2,1});  // 0 -> 2,1 (popularty order)
            friendGraph.emplace_back(std::vector<IndexType>{0});	// 1 -> 0
            friendGraph.emplace_back(std::vector<IndexType>{0,3});	// 2 -> 0,3 (popularity order)
            friendGraph.emplace_back(std::vector<IndexType>{2}); 	// 3 -> 2
            std::vector<uint8_t> raw { friendGraph.compact() };
            friendGraph.clear();
            IndexType length { friendGraph.load(raw) };

            THEN("Compact its contain must be the amount needed for its length and its payload") {
                REQUIRE( length == 4 );
                REQUIRE( friendGraph.size() == 4 );
                REQUIRE( friendGraph[0][0] == 2 );
                REQUIRE( friendGraph[0][1] == 1 );
                REQUIRE( friendGraph[0].size() == 2 );
                REQUIRE( friendGraph[0].capacity() == 2 );
                REQUIRE( friendGraph[1][0] == 0 );
                REQUIRE( friendGraph[1].size() == 1 );
                REQUIRE( friendGraph[1].capacity() == 1 );
                REQUIRE( friendGraph[2][0] == 0 );
                REQUIRE( friendGraph[2][1] == 3 );
                REQUIRE( friendGraph[2].size() == 2 );
                REQUIRE( friendGraph[2].capacity() == 2 );
                REQUIRE( friendGraph[3][0] == 2 );
                REQUIRE( friendGraph[3].size() == 1 );
                REQUIRE( friendGraph[3].capacity() == 1 );
            }
        }
   }
}
