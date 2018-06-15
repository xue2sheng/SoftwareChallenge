#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "file.hpp" 

using namespace SoftwareChallenge;

SCENARIO("Process human-friendly inputs", "[file]") {

	GIVEN("A human-friendly input line of two friends") {
	
		Collection network{};
		REQUIRE(network.size() == 0);

		network.add("John", "Ian");
		REQUIRE(network.size() == 2);

		WHEN("They both were added sequentially") {

			auto John = network["John"];
			REQUIRE(John.index == 0);

			auto Ian = network["Ian"];
			REQUIRE(Ian.index == 1);

			THEN("They both must be friends") {
				for (const auto& i : John) {
					REQUIRE(i == Ian.index);
				}
				for (const auto& j : Ian) {
					REQUIRE(j == John.index);
				}
			}
		}
	}

	GIVEN("A human-friendly input file") {
	
		Collection network{};
		REQUIRE(network.size() == 0);

		network.process("C:/Users/user/Code/SoftwareChallenge/data/test01.txt");

		WHEN("Reed expected number of members") {
			
			REQUIRE(network.size() == 420);

			//MYLES_JEFFCOAT, LANNY_TIBURCIO
			auto Lanny = network["LANNY_TIBURCIO"];
			REQUIRE(Lanny.size() > 0);
			auto Nyles = network["MYLES_JEFFCOAT"];
			REQUIRE(Nyles.size() > 0);

			THEN("MYLES_JEFFCOAT and LANNY_TIBURCIO should be friends") {
				REQUIRE( Nyles.end() != Nyles.find(Lanny.index) );
			}
		}
	}


	
}