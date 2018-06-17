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
