#include <file.hpp>
#include <fstream>
#include <string>

using namespace SoftwareChallenge;

void Member::add(const IndexType& new_friend) { insert(new_friend); } 

void Collection::add(const std::string& key, const std::string& new_friend)
{
	// ignore friends of themselves or empty names 
	if ((key == new_friend) || (key.empty()) || (new_friend.empty())) { return;  }

	// that key got already some valid index???
	auto key_info = find(key);
	if (key_info == end()) {
		// key should be added first to obtain its index
		auto expected_index = size();
		auto[new_key_info, inserted] = insert(std::pair<const std::string, Member>(key, Member{}));
		new_key_info->second.index = expected_index;
		key_info = new_key_info;
	}

	// that new friend got already some valid index???
	auto friend_info = find(new_friend);
	if (friend_info == end()) {
		// friend should be added first to obtain its index
		auto expected_index = size();
		auto[new_friend_info, inserted] = insert(std::pair<const std::string, Member>(new_friend, Member{}));
		new_friend_info->second.index = expected_index;
		friend_info = new_friend_info;
	}

	// friendship is considered bidirectional
	key_info->second.add(friend_info->second.index);
	friend_info->second.add(key_info->second.index);
}

// RAII wrapper to open/close file
struct RAII {
	std::ifstream file;
	RAII(const std::string& file_name) {
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(file_name.c_str());
	}
	~RAII() {
		file.close();
	}
};

bool Collection::process(const std::string& file_name)
{
	try {
		RAII raii(file_name.c_str());
		std::string line;
		while (std::getline(raii.file, line)) {

			std::string key;
			std::string new_friend;

			// only liees with ","
			size_t pos{ line.find(",") };
			if( pos != std::string::npos) {
				key = line.substr(0, pos);
				line.erase(0, pos + 1); // 1 == length of ","
				new_friend = line.substr(0, pos);
			}

			// trim whitespaces
			key.erase(0, key.find_first_not_of(' ')); //prefixing spaces
			key.erase(key.find_last_not_of(' ') + 1); //surfixing spaces
			new_friend.erase(0, new_friend.find_first_not_of(' ')); //prefixing spaces
			new_friend.erase(new_friend.find_last_not_of(' ') + 1); //surfixing spaces

			// add that relationship
			add(key, new_friend);
		}
	}
	catch (const std::ifstream::failure& ) {
		return false;
	}

	return true;
}