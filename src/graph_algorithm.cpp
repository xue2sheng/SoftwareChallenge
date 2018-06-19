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
#include <sstream>

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
    if( A == B ) {
        return { true, A + " You're supposed to be friend of yourself", 0 };
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
    TiesBFS tiesA {doneA, doneB, indexB, friendGraph, visitedA, visitedB, indexA, "A"};
    TiesBFS tiesB {doneB, doneA, indexA, friendGraph, visitedB, visitedA, indexB, "B"};

    // launch one search
    std::thread searchA(std::ref(tiesA));
    std::thread searchB(std::ref(tiesB));
    searchA.join();
    searchB.join();

    if( (tiesB.getDistance() == tiesA.getDistance()) && (tiesA.getDistance() == INDEX_MAX) ) {

        return { false, searchId + " It seems they don't have a link of friends between them", INDEX_MAX };

    } else {

        std::stringstream ss;
        ss << searchId;
        ss << "    " << std::min(visitedA[indexB],visitedB[indexA]) << " ties should sufice";
        ss << "    threadA=" << visitedA[indexB] << " threadB=" << visitedB[indexA];

        std::string commonA;
        if( auto[success, name] = name2index.getName( tiesA.getCommon() ); success ) {
            if( (tiesA.getDistance() != INDEX_MAX) || (tiesA.getCommon() != indexB) ) {
                ss <<  " commonA=" << name << "[" << tiesA.getCommon() << "]<" << tiesA.getDistance() << ">";
            }
        }

        std::string commonB;
        if( auto[success, name] = name2index.getName( tiesB.getCommon() ); success ) {
            if( (tiesB.getDistance() != INDEX_MAX) || (tiesB.getCommon() != indexB) ) {
                ss <<  " commonA=" << name << "[" << tiesB.getCommon() << "]<" << tiesB.getDistance() << ">";
            }
        }

        if( (visitedA[indexB] == 0) && (visitedB[indexA] != 0) ) {
            return { true, ss.str(), visitedB[indexA] };
        }

        if( (visitedA[indexB] != 0) && (visitedB[indexA] == 0) ) {
            return { true, ss.str(), visitedA[indexB] };
        }

        return { true, ss.str(), std::min(visitedA[indexB],visitedB[indexA]) };
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
     while( ! queue.empty() && ! myDone.load() ) {

         // get the next one
         auto next { queue.front() };
         queue.pop();

         // common friend or reach the oher side???
         if( othersVisited[next.first].load() ) {

            // needed that initial distance be INDEX_MAX
            // supposed that A and B cannot be the same
            if( (distance == 0) || (next.second < distance) ) { common = next.first; distance = next.second; }
         }

         // check it out if target was reached
         if( next.first == target ) {

             // done with this thread
             myDone.store(true);

             // warn the other thread about the distance
             othersVisited[start] = next.second;

             // done!
             break;
         }

         // if the other thread has finished, just not add more items to the queue and clean it up as soon as possible
         if( ! othersDone.load() ) {

             // go through all her/his friends
             step++;
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
