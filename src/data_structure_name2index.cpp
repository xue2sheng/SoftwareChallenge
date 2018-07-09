/**
* @file data_structure_name2index.cpp
* @author Andres Sevillano
* @date June 2018
* @brief Utilities to compact computer-friendly collection info. Focus on string name and indexes.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "data_structure_basic.hpp"
#include "data_structure_name2index.hpp"

using namespace SoftwareChallenge;

// just needed room for number of members + name_index info
size_t NameIndex::neededBytes() const
{
    return ( sizeof(IndexType) + ( size() * (sizeof(NameType) + sizeof(IndexType)) ) );
}

std::tuple<bool, IndexType> NameIndex::getIndex(const std::string& name) const
{
    const auto& found = find(name);
    if (found == end()) { return { false, INDEX_MAX }; }

    return { true, found->second };
}

// slow. It's only used by debugging stuff
std::tuple<bool,std::string> NameIndex::getName(const IndexType index) const
{
     for(const auto& i : (*this)) { if( (i.second) == index ) { return { true, i.first }; } }
     return { false, ""};
}

std::vector<uint8_t> NameIndex::compact() const
{
    // reserve only that needed room
    size_t needed { neededBytes() };
    std::vector<uint8_t> result(needed);
    result.reserve(needed);

    // clean up
    memset(result.data(), 0, needed);


    // First number of members
    IndexType length = static_cast<IndexType>(size());
    std::memcpy(result.data(), static_cast<const void*>(&length), sizeof(IndexType));

    // THen name->index pairs
    uint8_t* dest = result.data() + sizeof(IndexType);
    for(const auto& i : (*this)) {

        // name
        std::memcpy(dest, static_cast<const void*>(i.first.data()), sizeof(NameType));
        dest += sizeof(NameType);

        // index avoiding big/little endian as much as possible
        UIndexType data {0};
        data.raw = {i.second};
        data.compact(dest);
        dest += sizeof(IndexType);
    }

    return result;
}

IndexType NameIndex::load(std::vector<uint8_t>& raw)
{
    // nothing to work with
    if( raw.empty() ) { return 0; }

    // sanity check
    size_t raw_size = raw.size();
    if( raw_size < sizeof(IndexType) ) { return 0; }
    IndexType length { *reinterpret_cast<IndexType*>(raw.data()) };
    if( length == 0 ) { return 0; }

    // TODO: business logic check
    IndexType expected_length = static_cast<IndexType>(
            // don't count intial length info
            (raw_size - sizeof(IndexType)) /
            // every element is a pair name->index
            (sizeof(NameType) + sizeof(IndexType))
    );
    if( static_cast<size_t>(length) != expected_length ) { return 0; }

    // clean current data to avoid messing about with stale info
    clear();

    for(IndexType i = 0; i<length; ++i) {

        // don't forget to skip initial length
        const uint8_t* ptr = raw.data() + sizeof(IndexType) + (i * (sizeof(NameType) + sizeof(IndexType)));

        // name
        const char* name = reinterpret_cast<const char*>(ptr);
        ptr += sizeof(NameType);

        // index avoiding big/little endian as much as possible
        UIndexType data {0};
        data.load(ptr);

        // emplace payload
        emplace( name, data.raw );
    }

    return length;
}

