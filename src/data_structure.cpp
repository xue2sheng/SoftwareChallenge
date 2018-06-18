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
#include <string>
#include "data_structure.hpp"


#include <iostream>

using namespace SoftwareChallenge;

void Member::add(const IndexType& new_friend) { insert(new_friend); }

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

std::vector<uint8_t> NameIndex::compact() const
{
    // reserve only that needed room
    size_t needed { neededBytes() };
    std::vector<uint8_t> result(needed);
    result.reserve(needed);

    // First number of members
    IndexType length = static_cast<IndexType>(size());
    std::memcpy(result.data(), static_cast<const void*>(&length), sizeof(size_t));

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

IndexType NameIndex::load(std::vector<uint8_t>& raw)
{
    // nothing to work with
    if( raw.empty() ) { return 0; }

    // sanity check
    size_t raw_size = raw.size();
    if( raw_size < sizeof(size_t) ) { return 0; }
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

        // emplace payload
        const char* name = reinterpret_cast<const char*>(ptr);
        const IndexType index { *(ptr + sizeof(NameType)) };
        emplace( name, index);
    }

    return length;
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structrue:
*   2a) one IndexType with the number of friends for this member (m)
*   2b) m IndexType as friends indexes for this member
*/
size_t FriendGraph::neededBytes() const
{
    size_t relations {0};
    for(const auto& i : (*this) ) { relations += i.size(); }
    return ( sizeof(IndexType) + (size() * (sizeof(size_t) + sizeof(IndexType))) + (relations * (sizeof(IndexType))) );
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structrue:
*   2a) one IndexType with the number of friends for this member (m)
*   2b) m IndexType as friends indexes for this member
*/
std::vector<uint8_t> FriendGraph::compact() const
{
    // reserve only that needed room
    size_t needed { neededBytes() };
    std::vector<uint8_t> result(needed);
    result.reserve(needed);

    // 0) one IndexTyep (4 bytes) with the number of members (n)
    IndexType length = static_cast<IndexType>(size());
    std::memcpy(result.data(), static_cast<const void*>(&length), sizeof(IndexType));

    // 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
    uint8_t* base = result.data() + sizeof(IndexType); // --> beginning of 1)
    size_t offset = sizeof(IndexType) + (size() * sizeof(size_t));  // values of 1) are some kind of internal offset to point to 2)
    uint8_t* dest = result.data() + offset;  // --> ending of 1)

    // relationship graph
    for(const auto& i : (*this)) {

        /*** 1) n size_t (8 bytes each) for pointing inside this uint8_t* to ***/

        // 1) pointing to 2)
        std::memcpy(base, static_cast<const void*>(&offset), sizeof(size_t));
        // next element in 1)
        base += sizeof(size_t);

        /*** 2) n set of IndexType following the structrue: ***/

        // 2a) one IndexType with the number of friends for this member (m)
        IndexType num = static_cast<IndexType>(i.size());
        std::memcpy(dest, static_cast<const void*>(&num), sizeof(IndexType));
        dest += sizeof(IndexType);

        // 2b) m IndexType as friends indexes for this member
        std::memcpy(dest, static_cast<const void*>(i.data()), i.size()*sizeof(IndexType));
        dest += i.size()*sizeof(IndexType);

        // offset for size_t at base
        offset += ( sizeof(IndexType) + i.size()*sizeof(IndexType) );
    }

    return result;
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structrue:
*   2a) one IndexType with the number of friends for this member (m)
*   2b) m IndexType as friends indexes for this member
*/
IndexType FriendGraph::load(std::vector<uint8_t>& raw)
{
    // nothing to work with
    if( raw.empty() ) { return 0; }

    // sanity check
    size_t raw_size = raw.size();
    // at least size
    if( raw_size < sizeof(IndexType) ) { return 0; }
    IndexType length { *reinterpret_cast<IndexType*>(raw.data()) };
    if( length == 0 ) { return 0; }
    // at least size + offsets
    if( raw_size < (sizeof(IndexType) + ( length * sizeof(size_t))) ) { return 0; }

    // clean current data to avoid messing about with stale info
    clear();

    // reserve room enough
    reserve(static_cast<size_t>(length));

    // add relationships
    for(size_t i = 0; i < static_cast<size_t>(length); ++i) {
        // offset to point to size of the set of friends
        size_t offset = static_cast<size_t>(*(raw.data() + sizeof(IndexType) + (i*sizeof(size_t))));
        // bytes needed to keep the set of friend (relationships)
        IndexType num = static_cast<IndexType>(*(raw.data() + offset));
        emplace_back( std::vector<IndexType>(num) );
        std::memcpy( operator[](i).data(), &raw[offset + sizeof(IndexType)], num * sizeof(IndexType) );
    }

    return length;
}
