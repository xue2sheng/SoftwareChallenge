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
#include <queue>
#include <thread>
#include <utility>
#include <mutex>
#include <sstream>

#include "graph_algorithm.hpp"
#include "preprocess.hpp"

std::mutex guard_mutex;

/**** slow thread debug ****/
#include <iostream>
void trace(const std::string& threadId, const std::string& message) {
    std::lock_guard<std::mutex> lock(guard_mutex);
    std::cout << threadId << ": " << message << std::endl;
}

using namespace SoftwareChallenge;

std::tuple<bool, std::string, IndexType, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> load(const std::string& filename)
{
    Collection collection;
    auto[ success, hint, num_of_members, name2index_size, friendGraph_size ] = collection.load(filename);

    return { success, hint, num_of_members, std::move(collection.name2index), std::move(collection.friendGraph) };
}

std::tuple<bool, std::string, IndexType> searchFriends(const std::string& A, const std::string& B, const NameIndex& name2index, const FriendGraph& friendGraph)
{
    if( A == B ) {
        return { true, A + ", you're supposed to be friend of yourself, don't care to what social network you belong to", 0 };
    }

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

    // Are direct friends?
    if( friendGraph.areFriends(indexA, indexB) )
    {
        return { true, searchId + " They are direct friends", 0 };
    }

    // avoid cycles in those searchs
    Visited visitedA(length);
    for(auto&& i : visitedA) { i.store(0); } // just in case its default value it's not zero
    Visited visitedB(length);
    for(auto&& i : visitedB) { i.store(0); } // just in case its default value it's not zero

    // Being friendships bidirectional, we'd better launch two concurrent search from each side and meet in the middle
    TiesBFS tiesA {indexB, friendGraph, visitedA, visitedB, indexA, "A"};
    TiesBFS tiesB {indexA, friendGraph, visitedB, visitedA, indexB, "B"};

    // launch one search
    std::thread searchA(std::ref(tiesA));
    std::thread searchB(std::ref(tiesB));
    searchA.join();
    searchB.join();

    /***** debuggin *****/
//    if( true ) {
 //       std::cout << "----- search A -----" << tiesA.getCommon() << "[" << tiesA.getDistance() << "] ==> " << visitedB[tiesA.getCommon()] << std::endl;
  //      std::cout << "----- search B -----" << tiesB.getCommon() << "[" << tiesB.getDistance() << "] ==> " << visitedA[tiesB.getCommon()] << std::endl;
   //     std::cout << "----- visited ------ <" << visitedA[indexB] << "," << visitedB[indexA] << ">" << std::endl;
    //}

    std::stringstream ss;
    ss << searchId;
    ss << " threadA=" << visitedA[indexB] << " threadB=" << visitedB[indexA] << " min=" << std::min(visitedA[indexB],visitedB[indexA]);

    std::string commonA;
    if( auto[success, name] = name2index.getName( tiesA.getCommon() ); success ) {
        ss <<  " commonA=" << name << "[" << tiesA.getCommon() << "]<" << tiesA.getDistance() << ">";
    }

    std::string commonB;
    if( auto[success, name] = name2index.getName( tiesB.getCommon() ); success ) {
        ss << " commonB=" << name << "[" << tiesB.getCommon() << "]<" << tiesB.getDistance() << ">";
    }

    return { true, ss.str(), std::min(visitedA[indexB],visitedB[indexA]) };
}

TiesBFS::TiesBFS(const IndexType targetPoint, const FriendGraph& friendGraph, Visited& mine, Visited& others,
                 const IndexType startPoint, const std::string& threadId_)
    : target{targetPoint},  graph{friendGraph},
      myVisited{mine}, othersVisited{others},
      start{startPoint}, threadId{threadId_}
{
    // worst scenario
    common = target;
    distance = INDEX_MAX;
}

IndexType TiesBFS::getDistance() const { return distance; };
IndexType TiesBFS::getCommon() const { return common; };

void TiesBFS::operator()()
{
     // Typical queue for BFS algorithms
     std::queue<std::pair<IndexType, IndexType>> queue;

     // number of steps
     IndexType step {0};

     // Get ready for the initial one
     myVisited[start].store(true);
     queue.emplace(start, step);

     // take a walk
     while( ! queue.empty() ) {

         // get the next one
         auto next { queue.front() };
         queue.pop();

         // common friend or reach the oher side???
         if( othersVisited[next.first].load() ) {
            //trace(threadId, "reached other or just some common " + std::to_string(next.first) + " queued as " + std::to_string(next.second));

            // needed that initial distance be INDEX_MAX
            // supposed that A and B cannot be the same
            if( (distance == 0) || (next.second < distance) ) { common = next.first; distance = next.second; }
         }

         //trace(threadId, "pop " + std::to_string(next.first) + "[" + std::to_string(step) + "]");
         step++;

         // go through all her/his friends
         for(const auto& i : graph[next.first]) {

             // keep on searching
             if( ! myVisited[i].load() ) {
                 myVisited[i].store(step);
                 //trace(threadId, ">>> " + std::to_string(i) + "<" + std::to_string(step) + ">");
                 queue.emplace(i, step);
             }
         }

     } // while
}
