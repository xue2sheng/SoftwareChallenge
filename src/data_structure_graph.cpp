/**
* @file data_structure_graph.cpp
* @author Andres Sevillano
* @date June 2018
* @brief Utilities to compact computer-friendly collection info. Focus on indexes' graph.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>

#include "data_structure_basic.hpp"
#include "data_structure_graph.hpp"

using namespace SoftwareChallenge;

bool FriendGraph::areFriends(const IndexType A, const IndexType B) const
{
    size_t length {size()};
    if( (length == 0) || (A >= length) || (B >= length) ) { return false; }

    // alias
    auto const& setA { operator[](A) };
    return (setA.end() != std::find(setA.begin(), setA.end(), B));
}

// slow for tests
bool FriendGraph::operator ==(const FriendGraph& other) const
{
    if( size() != other.size() )  { return false; }
    for(auto i = 0; i<size(); ++i) {
        if( other[i].size() != operator[](i).size() ) { return false; }
        for(auto j = 0; j < other[i].size(); ++j ) {
            if( other[i][j] != operator[](i).operator[](j) ) { return false; }
        }
    }
    return true;
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structure:
*   2a) one IndexType with the number of friends for this member (m)
*   2b) m IndexType as friends indexes for this member
*/
size_t FriendGraph::neededBytes() const
{
    size_t result {0};
    result += sizeof(IndexType); // 0
    result += ( size() * sizeof(size_t) ); // 1
    for(const auto& i : (*this)) {
        result += sizeof(IndexType); // 2a
        result += ( i.size() * sizeof(IndexType) );
    }
    return result;
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structure:
*   2a) one IndexType with the number of friends for this member (m)
*   2b) m IndexType as friends indexes for this member
*/
std::vector<uint8_t> FriendGraph::compact() const
{
    // reserve only that needed room
    size_t needed { neededBytes() };
    std::vector<uint8_t> result(needed);
    result.reserve(needed);

    // clean up
    memset(result.data(), 0, needed);

    // 0) one IndexTyep (4 bytes) with the number of members (n)
    IndexType length = static_cast<IndexType>(size());
    std::memcpy(result.data(), static_cast<const void*>(&length), sizeof(IndexType));

    size_t relations {0};
    IndexType index {0};
    size_t base = sizeof(IndexType) + size() * sizeof(size_t);
    for(const auto& i : (*this) ) {

        // 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
        size_t pointer = base + index * sizeof(IndexType) + relations * sizeof(IndexType);
        std::memcpy( result.data() + sizeof(IndexType) + index * sizeof(size_t),
                     static_cast<const void*>(&pointer),
                     sizeof(size_t)
        );

        // 2a) one IndexType with the number of friends for this member (m)
        UIndexType num_of_friends {0};
        num_of_friends.raw = static_cast<IndexType>(i.size());
        num_of_friends.compact( result.data() + pointer );

        // 2b) m IndexType as friends indexes for this member
        uint8_t* dest = result.data() + pointer + sizeof(IndexType);
        for(size_t j = 0; j < num_of_friends.raw; ++j) {

            // index avoiding big/little endian as much as possible
            UIndexType data {0};
            data.raw = {i[j]};
            data.compact(dest);
            dest += sizeof(IndexType);
        }

        // next
        ++index;
        relations += i.size();
    }
    return result;
}

/*
* 0) one IndexTyep (4 bytes) with the number of members (n)
* 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
* 2) n set of IndexType following the structure:
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
    UIndexType length {0};
    length.load(raw.data());

    if( length.raw == 0 ) { return 0; }
    // at least size + offsets
    if( raw_size < (sizeof(IndexType) + ( length.raw * sizeof(size_t))) ) { return 0; }

    // clean current data to avoid messing about with stale info
    clear();

    // reserve room enough
    reserve(static_cast<size_t>(length.raw));

    // add relationships
    for(size_t i = 0; i < static_cast<size_t>(length.raw); ++i) {

        // offset to point to size of the set of friends
        const void* pointer = raw.data() + sizeof(IndexType) + (i * sizeof(size_t));
        size_t offset {0};
        std::memcpy( &offset, reinterpret_cast<const void*>(pointer), sizeof(size_t));

        // index avoiding big/little endian as much as possible
        UIndexType num_of_friends {0};
        num_of_friends.load(raw.data() + offset);

        // bytes needed to keep the set of friend (relationships)
        std::vector<IndexType> temp(num_of_friends.raw);
        temp.resize(num_of_friends.raw);
        std::memcpy(temp.data(), &raw[offset + sizeof(IndexType)], num_of_friends.raw * sizeof(IndexType) );

        for(size_t j = 0; j < static_cast<size_t>(num_of_friends.raw); ++j) {
            UIndexType index {0};
            index.load( raw.data() + offset + sizeof(IndexType) + (j*sizeof(IndexType)) );
            temp.emplace_back( std::move(index.raw) );
        }

        emplace_back( std::move(temp) );
    }

    return length.raw;
}
