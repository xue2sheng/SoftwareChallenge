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
#include "commandline.hpp"

using namespace SoftwareChallenge;

std::tuple<bool, std::string, bool, size_t, CoupleList, NameIndex, FriendGraph> preprocess(int argc, char** argv)
{
    // user input
    auto[file_name, binary, compact_file_name, stats, generate, coupleList ] = commandline_arguments(argc, argv);

    if (file_name.empty()) {
        return{ false, "Empty file name", stats, 0, coupleList, NameIndex{}, FriendGraph{} };
	}

    // this way all collection non-compact will be freed in a RAII way
    Collection network;

    // No need of preprocessing
    if( binary ) {

        auto[ success, hint, number_of_members, name2index_length, FriendGraph_length ] = network.load(file_name);
        if( ! success ) {
            return{ false, hint, stats, 0, coupleList, NameIndex{}, FriendGraph{} };
        }

        return { true, hint, false, number_of_members, coupleList, network.name2index, network.friendGraph };
    }

    // process file
	if (auto[success, hint] = network.process(file_name); !success) {
        return { false, hint, stats, network.size(), coupleList, NameIndex{}, FriendGraph{} };
	}

    // compact that info
    auto[success, hint, number_of_members, name2index, friendGraph] = network.compact();

    // store compact version only if required
    if ( success && generate && ! compact_file_name.empty() ) {

        // try to store that compact info
        if( auto[stored, issues, length, ni_length, fg_length ] = network.store(compact_file_name); !stored) {
            return { false, issues, stats, number_of_members, coupleList, name2index, friendGraph };
        } else {
            hint += " stored in file " + compact_file_name;
        }
    }

    return { success, hint, stats, number_of_members, coupleList, name2index, friendGraph };
}

void Collection::reset()
{
    clear(); name2index.clear(); friendGraph.clear(); relations = 0;
    friends_max = friends_min = name_max = name_min = 0; popular_max = popular_min = "";
}

size_t Collection::relationships() const { return relations; }
Member::size_type Collection::friendsMax() const { return friends_max; }
Member::size_type Collection::friendsMin() const { return friends_min; }
std::string::size_type Collection::nameMax() const { return name_max; }
std::string::size_type Collection::nameMin() const { return name_min; }
std::string Collection::popularMax() const { return popular_max; }
std::string Collection::popularMin() const { return popular_min; }

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

std::tuple<bool, std::string, size_t, NameIndex, FriendGraph> Collection::compact()
{
    name2index.clear();
    friendGraph.clear();
    relations = 0;
    friends_max = 0;
    friends_min = INDEX_MAX;

    if (size() == 0) {
        return { false, "Nothing to compact", 0, name2index, friendGraph };
	}

    // reserve space to avoid vector resizing as much as possible
    friendGraph.resize(size());

	for (const auto& item : *this) {

		// alias
		const auto& name{ item.first };
		const auto& index{ item.second.index };
		const auto& friends{ item.second };
		const auto& num{ friends.size() };

		// collect name->index map
		name2index[name] = index;

        // collect friend relationships
        friendGraph[index] = std::vector<IndexType>(friends.begin(), friends.end());
        friendGraph[index].resize(num);

        // stats for future optimization
        relations += num;
		if (friends_min > num) { friends_min = num; popular_min = name;  }
		if (friends_max < num) { friends_max = num; popular_max = name;  }
	}

    // order friend sets from more popular to less one
    // this is going to take time in order to spare it once we only work with the compact version
    // so hopefully it'll only be done once:  that means computer-friendly ;)
    for (auto&& i : friendGraph) {
        std::sort( i.begin(), i.end(), [&](const auto& a, const auto& b) { return (friendGraph[a].size() > friendGraph[b].size()); });
    }

    // at least there should be some member in the collection
	return {
		(size() > 0),
        "size=" + std::to_string(size()) +
        " relationships=" + std::to_string(relations) +
		" name_min=" + std::to_string(name_min) + 
		" name_max=" + std::to_string(name_max) + 
		" popular_min=" + popular_min + 
		" friends_min=" + std::to_string(friends_min) +
		" popular_max=" + popular_max + 
		" friends_max=" + std::to_string(friends_max),
		size(),
        name2index,
        friendGraph
	};
}

// RAII wrapper to open/close file
struct RAII_binary {
    std::ofstream file;
    RAII_binary(const std::string& file_name) : file(file_name, std::fstream::binary) {}
    ~RAII_binary() { if (file.is_open()) { file.close(); } }
};

std::tuple<bool, std::string, IndexType, size_t, size_t> Collection::store(const std::string& file_name)
{
    // basic checks
    if(file_name.empty())  { return {false, "Empty file name", 0, 0, 0}; }
    IndexType length { static_cast<IndexType>(size()) };
    if(length == 0) { return {false, "Nothing to store", length, 0, 0}; }

    // First compact that info
    if(name2index.size() == 0 || friendGraph.size() == 0) {

        if( auto[success, hint, ignore, dont_care, whatever] = compact(); !success) {
            return { false, "Store failed due to " + hint, 0, 0, 0 };
        }

    }

    // extra check
    if( (name2index.size() != friendGraph.size()) || (name2index.size() != size()) ) {
        return {false, "Mismatched NameIndex and FriendGraph structures", length, name2index.size(), friendGraph.size()};
    }

    // mainly for debugging and double checks
    size_t name2index_length { 0 };
    size_t friendGraph_length { 0 };

    try {
        RAII_binary raii(file_name.c_str());

        // Number of members ---> FIRST ELEMENT (IndexType)
        raii.file.write( reinterpret_cast<const char*>(&length), sizeof(IndexType) );

        // NameIndex
        auto compact_name2index { name2index.compact() };
        name2index_length = compact_name2index.size();

        // NameIndex size in bytes to be stored in that binary file ----> SECOND ELEMENT (size_t)
        raii.file.write( reinterpret_cast<const char*>(&name2index_length), sizeof(size_t) );

        // FriendGraph payload
        auto compact_friendGraph { friendGraph.compact() };
        friendGraph_length = compact_friendGraph.size();

        // FriendGraph size in bytes to be stored in that binary file ----> THIRD ELEMENT (size_t)
        raii.file.write( reinterpret_cast<const char*>(&friendGraph_length), sizeof(size_t) );

        // write name2index payload -----> FOURTH ELEMENT ( n * (size_t + NameIndex) )
        raii.file.write( reinterpret_cast<const char*>(compact_name2index.data()), name2index_length );

        // write friendGraph payload -----> FIFTH ELEMENT ( n * (IndexType + different m * IndexType) )
        raii.file.write( reinterpret_cast<const char*>(compact_friendGraph.data()), friendGraph_length);

    }
    catch (const std::ofstream::failure& e) {
        return { false, e.what(), length, name2index_length, friendGraph_length };
    }
    catch (...) {
        return { false, "Something went bananas with binary " + file_name, length, name2index_length, friendGraph_length };
    }

    return { true, "Stored compact format at " + file_name, length, name2index_length, friendGraph_length };
}

// RAII wrapper to open/close file
struct RAII_input_binary {
    std::ifstream file;
    RAII_input_binary(const std::string& file_name) : file(file_name, std::fstream::binary) {}
    ~RAII_input_binary() { if (file.is_open()) { file.close(); } }
};

std::tuple<bool, std::string, IndexType, size_t, size_t> Collection::load(const std::string& file_name)
{
    // number of members
    IndexType length{0};

    // size
    size_t name2index_length {0};

    // size
    size_t friendGraph_length {0};

    try {
        RAII_input_binary raii(file_name.c_str());

        // TODO: so weak sanity check, getting its file size with tellg()

        // Number of members ---> FIRST ELEMENT (IndexType)
        raii.file.read( reinterpret_cast<char*>(&length), sizeof(IndexType) );

        // NameIndex size in bytes ---> SECOND ELEMENT (size_t)
        raii.file.read( reinterpret_cast<char*>(&name2index_length), sizeof(size_t) );

        // FriendGraph size in bytes to be stored in that binary file ----> THIRD ELEMENT (size_t)
        raii.file.read( reinterpret_cast<char*>(&friendGraph_length), sizeof(size_t) );

        // NameIndex payload  -----> FOURTH ELEMENT ( n * (size_t + NameIndex) )
        std::vector<uint8_t> compact_name2index (name2index_length);
        raii.file.read( reinterpret_cast<char*>(compact_name2index.data()), name2index_length );
        IndexType name2index_expected_length = name2index.load(compact_name2index);

        // extra check
        if( length != name2index_expected_length  ) {
            return { false, "Mismatch at NameIndex structure in binary file " + file_name, length, name2index_length, friendGraph_length };
        }

        // FindGraph payload   -----> FIFTH ELEMENT ( n * (IndexType + different m * IndexType) )
        std::vector<uint8_t> compact_friendGraph (friendGraph_length);
        raii.file.read( reinterpret_cast<char*>(compact_friendGraph.data()), friendGraph_length );
        IndexType friendGraph_expected_length = friendGraph.load(compact_friendGraph);

        // extra check
        if( length != friendGraph_expected_length  ) {
            return { false, "Mismatch at FriendGraph structure in binary file " + file_name, length, name2index_length, friendGraph_length };
        }

    }
    catch (const std::ifstream::failure& e) {
        return { false, e.what(), 0, name2index_length, friendGraph_length };
    }
    catch (...) {
        return { false, "Something went bananas with binary " + file_name, length, name2index_length, friendGraph_length };
    }

    return { true, "Loaded binary data from " + file_name, length, name2index_length, friendGraph_length };
}
