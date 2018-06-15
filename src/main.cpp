// main.cpp : Defines the entry point for the application.
//

#include "main.hpp"

using namespace std;
using namespace SoftwareChallenge;

int main()
{
	extern const char* SOFTWARE_CHALLENGE_VERSION;
	cout << SOFTWARE_CHALLENGE_VERSION << endl;

	Collection collection;
	collection.add("John", "Ian");
	collection.add("John", "Earl");
	
	for (const auto& i : collection) {
		cout << i.first.c_str() << " index=" << i.second.index << " size=" << i.second.size() << endl;
		for (const auto& j : i.second) {
			cout << "  friend=" << j << endl;
		}
	}

	/*
	Collection network;
	network.process("C:/Users/user/Code/SoftwareChallenge/data/test01.txt");
	for (const auto& i : network) {
		cout << i.first.c_str() << " -> " << i.second.index << endl;
	}
	cout << endl << "size()=" << network.size() << endl << endl;
	*/

	return 0;
}
