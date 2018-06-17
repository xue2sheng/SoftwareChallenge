/**
* @file preprocess.cpp
* @author Andres Sevillano 
* @date June 2018
* @brief Utilities to process human-friendly input file.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include "preprocess.hpp"

using namespace SoftwareChallenge;

std::tuple<bool, std::string, size_t, NameIndex> preprocess(const std::string& file_name)
{
	if (file_name.empty()) {
		return{ false, "Empty file name", 0, NameIndex{} };
	}

	// this way all collection non-compact will be freed in a RAII way
	Collection network;

	// process file
	if (auto[success, hint] = network.process(file_name); !success) {
		return { success, hint, network.size(), NameIndex{} };
	}

	// compact info
	return network.compact(); 
}

void Collection::reset() { clear(); friends_max = friends_min = name_max = name_min = 0; popular_max = popular_min = ""; }

inline Member::size_type Collection::friendsMax() const { return friends_max; }
inline Member::size_type Collection::friendsMin() const { return friends_min; }
inline std::string::size_type Collection::nameMax() const { return name_max; }
inline std::string::size_type Collection::nameMin() const { return name_min; }
inline std::string Collection::popularMax() const { return popular_max; }
inline std::string Collection::popularMin() const { return popular_min; }

Collection::iterator Collection::add(const std::string& key)
{
	// that key got already some valid index???
	auto key_info = find(key);
	if (key_info == end()) {
		// key should be added first to obtain its index
		auto expected_index = size();
		auto[new_key_info, inserted] = insert(std::pair<const std::string, Member>(key, Member{}));
		new_key_info->second.index = static_cast<IndexType>(expected_index);
		key_info = new_key_info;
	}
	return key_info;
}

void Collection::add(const std::string& key, const std::string& new_friend)
{
	// add if needed
	auto key_info = add(key);
	auto friend_info = add(new_friend);

	// friendship is considered bidirectional
	key_info->second.add(friend_info->second.index);
	friend_info->second.add(key_info->second.index);
}

// RAII wrapper to open/close file
struct RAII {
	std::ifstream file;
	RAII(const std::string& file_name) : file(file_name) {}
	~RAII() { if (file.is_open()) { file.close(); } }
};

std::tuple<bool, std::string> Collection::process(const std::string& file_name)
{
	try {
		RAII raii(file_name.c_str());
		std::string line;
		while (std::getline(raii.file, line)) {

			// some checks
			if (line.empty()) { return { false,"Empty line" }; }

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
			if (line.size() < 3) { return { false,"Missing one valid name at least" }; }

			std::string key;
			std::string new_friend;

			// only lines with ","
			size_t pos{ line.find(",") };
			if (pos == std::string::npos) { return { false, "Some line missed required comma" }; }

			key = line.substr(0, pos);
			//if (key.empty()) { continue; }
			if (key.empty()) { return { false, "Empty key name" }; }

			line.erase(0, pos + 1); // 1 == length of ","
			new_friend = line;
			if (new_friend.empty()) { return { false, "Empty friend name" }; }

			// stats for future optimization
			if (name_min > key.size()) { name_min = key.size(); }
			if (name_min > new_friend.size()) { name_min = new_friend.size(); }
			if (name_max < key.size()) { name_max = key.size(); }
			if (name_max < new_friend.size()) { name_max = new_friend.size(); }

			// add that relationship
			add(key, new_friend);
		}
	}
	catch (const std::ifstream::failure& e) {
		return { false, e.what() };
	}
	catch (...) {
		return { false, "Something went bananas" };
	}

	// at least there should be some member in the collection 
	return {
		(size() > 0),
		"size=" + std::to_string(size()) + 
		" name_min=" + std::to_string(name_min) +
		" name_max=" + std::to_string(name_max)
	};
}

std::tuple<bool, std::string, size_t, NameIndex> Collection::compact()
{
	if (size() == 0) {
		return { false, "Nothing to compact", 0, NameIndex{} };
	}

	NameIndex name2index;

	for (const auto& item : *this) {

		// alias
		const auto& name{ item.first };
		const auto& index{ item.second.index };
		const auto& friends{ item.second };
		const auto& num{ friends.size() };

		// collect name->index map
		name2index[name] = index;

		// stats for future optimization
		if (friends_min > num) { friends_min = num; popular_min = name;  }
		if (friends_max < num) { friends_max = num; popular_max = name;  }
	}

	// at least there should be some member in the collection 
	return {
		(size() > 0),
		"size=" + std::to_string(size()) + 
		" name_min=" + std::to_string(name_min) + 
		" name_max=" + std::to_string(name_max) + 
		" popular_min=" + popular_min + 
		" friends_min=" + std::to_string(friends_min) +
		" popular_max=" + popular_max + 
		" friends_max=" + std::to_string(friends_max),
		size(),
		name2index	
	};

}

