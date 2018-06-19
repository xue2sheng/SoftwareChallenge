/**
* @file grpah_algorithm.cpp
* @author Andres Sevillano
* @date June 2018
* @brief Utilities to deal with computer-friendly info.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <iostream>
#include <algorithm>
#include <string>
#include "graph_algorithm.hpp"
#include "preprocess.hpp"

using namespace SoftwareChallenge;

std::tuple<bool, std::string, IndexType, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> load(const std::string& filename)
{
    Collection collection;
    auto[ success, hint, num_of_members, name2index_size, friendGraph_size ] = collection.load(filename);

    return { success, hint, num_of_members, std::move(collection.name2index), std::move(collection.friendGraph) };
}

std::tuple<bool, std::string, IndexType> searchFriends(const std::string& A, const std::string& B, const NameIndex& name2index, const FriendGraph& friendGraph)
{
    auto length { friendGraph.size() };
    auto num { name2index.size() };

    if( (length == 0) || (num == 0) ) { return { false, "Empty graph", 0 }; }
    if( length != num ) { return { false, "Mismatched NamIndex and FriendGraph structures", 0}; }

    auto [foundA, indexA] = name2index.getIndex(A);
    if( !foundA ) { return { false, "Not found A member in this social network", 0 }; }
    auto [foundB, indexB] = name2index.getIndex(B);
    if( !foundB ) { return { false, "Not found B member in this social network", 0 }; }

    // for debugging
    std::string searchId { A + "[" + std::to_string(indexA) + "]<-->" + B + "[" + std::to_string(indexB) + "]" };

    // avoid cycles in those searchs
    Visited visited(length);

    // Being friendships bidirectional, we'd better launch two concurrent search from each side and meet in the middle
    TiesBFS tiesA {friendGraph, visited, indexA};
    TiesBFS tiesB {friendGraph, visited, indexB};

    return { false, searchId + " Not implemented search yet", 0 };
}

TiesBFS::TiesBFS(const FriendGraph& friendGraph, Visited& visitedMembers, const IndexType startPoint)
    : graph{friendGraph}, visited{visitedMembers}, start{startPoint} {}

void TiesBFS::operator()()
{

}
