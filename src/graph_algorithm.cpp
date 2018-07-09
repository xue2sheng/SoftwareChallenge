/**
* @file graph_algorithm.cpp
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
#include <sstream>
#include <mutex>

#include "graph_algorithm.hpp"
#include "preprocess.hpp"

/** @brief To protect shared resources **/
std::mutex MUTEX;

using namespace SoftwareChallenge;

std::tuple<bool, std::string, IndexType, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> load(const std::string& filename)
{
    Collection collection;
    auto[ success, hint, num_of_members, name2index_size, friendGraph_size ] = collection.load(filename);

    return { success, hint, num_of_members, std::move(collection.name2index), std::move(collection.friendGraph) };
}

std::tuple<bool, std::string, IndexType> searchIndexes(const IndexType& A, const IndexType& B, const NameIndex& name2index, const FriendGraph& friendGraph)
{
    if( A == B ) {
        return { true, std::to_string(A) + " You're supposed to be friend of yourself", INDEX_MAX };
    }

    auto length { friendGraph.size() };
    auto num { name2index.size() };

    if( (length == 0) || (num == 0) ) { return { false, "Empty graph", INDEX_MAX }; }
    if( length != num ) { return { false, "Mismatched NamIndex and FriendGraph structures", INDEX_MAX}; }

    auto [foundA, nameA] = name2index.getName(A);
    if( !foundA ) { return { false, "Not found index " + nameA + " member in this social network", INDEX_MAX }; }
    auto [foundB, nameB] = name2index.getName(B);
    if( !foundB ) { return { false, "Not found index " + nameB + " member in this social network", INDEX_MAX }; }

    // for debugging
    std::string searchId { nameA + "[" + std::to_string(A) + "]<-->" + nameB + "[" + std::to_string(B) + "]" };

    // Are direct friends?
    if( friendGraph.areFriends(A, B) )
    {
        return { true, searchId + " They are direct friends", 0 };
    }

    // flag done
    std::atomic<bool> doneA;
    doneA.store(false);
    std::atomic<bool> doneB;
    doneB.store(false);

    // avoid cycles in those searchs
    Visited visitedA(length);
    for(auto&& i : visitedA) { i.store(0); } // just in case its default value it's not zero
    Visited visitedB(length);
    for(auto&& i : visitedB) { i.store(0); } // just in case its default value it's not zero

    // Being friendships bidirectional, we'd better launch two concurrent search from each side and meet in the middle
    TiesBFS tiesA {doneA, doneB, B, friendGraph, visitedA, visitedB, A, "A"};
    TiesBFS tiesB {doneB, doneA, A, friendGraph, visitedB, visitedA, B, "B"};

    // launch one search
    std::thread searchA(std::ref(tiesA));
    std::thread searchB(std::ref(tiesB));
    searchA.join();
    searchB.join();

    // extra details to report
    std::stringstream ss;
    ss << searchId << "   ";

    // result
    IndexType ties { std::min(tiesA.getDistance(), tiesB.getDistance()) };

    // any common link
    if( ties != INDEX_MAX ) {

        // result
        ss << ties << " should suffice  ";

        // avoid to report ugly numberfor this debugging info
        if( auto[success, name] = name2index.getName( tiesA.getCommon() ); success ) {
        if( (tiesA.getDistance() != INDEX_MAX) || (tiesA.getCommon() != B) ) {
             ss <<  " commonA=" << name << "[" << tiesA.getCommon() << "]<" << tiesA.getDistance() << ">";
            }
        }

        // avoid to report ugly numberfor this debugging info
        if( auto[success, name] = name2index.getName( tiesB.getCommon() ); success ) {
         if( (tiesB.getDistance() != INDEX_MAX) || (tiesB.getCommon() != A) ) {
             ss <<  " commonB=" << name << "[" << tiesB.getCommon() << "]<" << tiesB.getDistance() << ">";
         }
        }

        // success
        return { true, ss.str(), ties };
    }

    // without link
    return { false, searchId + " It seems they don't have a link of friends between them", INDEX_MAX };
}

std::tuple<bool, std::string, IndexType> searchFriends(const std::string& A, const std::string& B, const NameIndex& name2index, const FriendGraph& friendGraph)
{
    if( A == B ) {
        return { true, A + " You're supposed to be friend of yourself", INDEX_MAX };
    }

    auto length { friendGraph.size() };
    auto num { name2index.size() };

    if( (length == 0) || (num == 0) ) { return { false, "Empty graph", INDEX_MAX }; }
    if( length != num ) { return { false, "Mismatched NamIndex and FriendGraph structures", INDEX_MAX}; }

    try { // try to convert into indexes

        unsigned long longA = std::stoul(A);
        unsigned long longB = std::stoul(B);

        if( longA >= INDEX_MAX || longB >= INDEX_MAX || longA >= length || longB >= length ) {
            throw std::runtime_error("Index out of limit");
        }

        return searchIndexes(static_cast<IndexType>(longA), static_cast<IndexType>(longB), name2index, friendGraph);

    } catch(...) { // names

        auto [foundA, indexA] = name2index.getIndex(A);
        if( !foundA ) { return { false, "Not found " + A + " member in this social network", INDEX_MAX }; }
        auto [foundB, indexB] = name2index.getIndex(B);
        if( !foundB ) { return { false, "Not found " + B + " member in this social network", INDEX_MAX }; }

        return searchIndexes(indexA, indexB, name2index, friendGraph);
    }
}

TiesBFS::TiesBFS(std::atomic<bool>& myDone_, std::atomic<bool>& othersDone_,
                 const IndexType targetPoint, const FriendGraph& friendGraph,
                 Visited& mine, Visited& others,
                 const IndexType startPoint, const std::string& threadId_)
    : myDone{myDone_}, othersDone{othersDone_},
      target{targetPoint},  graph{friendGraph},
      myVisited{mine}, othersVisited{others},
      start{startPoint}, threadId{threadId_}
{
    // worst scenario
    common = INDEX_MAX;
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
     while( ! queue.empty() && ! myDone.load() ) {

         // double check history of visited nodes looking for shared ones between both threads
         for(IndexType i = 0; i < myVisited.size(); ++i) {

             if( myVisited[i].load() && othersVisited[i].load() && i != start && i != target ) {

                // be extra carefull
                std::lock_guard<std::mutex> lock(MUTEX);

                myDone.store(true);
                othersDone.store(true);
                common = i;
                distance = myVisited[i].load() + othersVisited[i].load();
                break;
             }
         }

         // get the next one
         auto next { queue.front() };
         queue.pop();

         // reached target?
         if( next.first == target ) {

                 // be extra carefull
                std::lock_guard<std::mutex> lock(MUTEX);

                myDone.store(true);
                othersDone.store(true);
                common = target;
                distance = myVisited[target].load();
                break;
         }

         // if the other thread has finished, just not add more items to the queue and clean it up as soon as possible
         if( ! othersDone.load() ) {

             // next level
             step++;

             // go through all her/his friends
             for(const auto& i : graph[next.first]) {

                 // keep on searching
                 if( ! myVisited[i].load() ) {
                    myVisited[i].store(step);
                    queue.emplace(i, step);
                 }
             }

         } // othersDone

     } // while

     // done
     myDone.store(true);
}
