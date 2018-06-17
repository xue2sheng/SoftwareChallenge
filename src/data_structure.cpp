/**
* @file data_structure.cpp
* @author Andres Sevillano 
* @date June 2018
* @brief Utilities to compact computer-friendly collection info.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "data_structure.hpp"

using namespace SoftwareChallenge;

void Member::add(const IndexType& new_friend) { insert(new_friend); }

// just needed room for number of members + name_index info
size_t NameIndex::neededBytes() const
{
    return ( sizeof(IndexType) + ( size() * (sizeof(NameType) + sizeof(IndexType)) ) );
}

std::vector<uint8_t> NameIndex::compact() const
{
    // reserve only that needed room
    size_t needed { neededBytes() };
    std::vector<uint8_t> result(needed);
    result.reserve(needed);

    // First number of memebers
    IndexType length = static_cast<IndexType>(size());
    std::memcpy(result.data(), static_cast<const void*>(&length), sizeof(IndexType));

    // THen name->index pairs
    uint8_t* dest = result.data() + sizeof(IndexType);
    for(const auto& i : (*this)) {
        std::memcpy(dest, static_cast<const void*>(i.first.data()), sizeof(NameType));
        dest += sizeof(NameType);
        std::memcpy(dest, static_cast<const void*>(&(i.second)), sizeof(IndexType));
        dest += sizeof(IndexType);
    }

    return result;
}

IndexType NameIndex::load(const std::vector<uint8_t>& raw)
{
    // nothing to work with
    if( raw.empty() ) { return 0; }

    // sanity check
    size_t raw_size = raw.size();
    if( raw_size < sizeof(IndexType) ) { return 0; }
    IndexType length { *raw.data() };

    // business logic check
    IndexType expected_length =
            // don't count intial length info
            (raw_size - sizeof(IndexType)) /
            // every element is a pair name->index
            (sizeof(NameType) + sizeof(IndexType));

    if( length != expected_length ) { return 0; }

    // clean current data to avoid messing about with stale info
    clear();

    for(IndexType i = 0; i<length; ++i) {

        // don't forget to skip initial length
        const uint8_t* ptr = raw.data() + sizeof(IndexType) + (i * (sizeof(NameType) + sizeof(IndexType)));

        // emplace payload
        const char* name = reinterpret_cast<const char*>(ptr);
        const IndexType index { *(ptr + sizeof(NameType)) };
        emplace( name, index);
    }

    return length;
}
