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

        WHEN("Non empty map") {

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
        }

	}
}
