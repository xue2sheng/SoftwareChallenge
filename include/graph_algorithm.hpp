/**
* @file graph_algorithm.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Process computer-friendly info as a typical graph BFS.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <atomic>
#include <queue>

#include "data_structure.hpp"

/**
* @brief load binary info 
* @param filename to load 
* @return true if success with extra hints. As well data structures
*/
std::tuple<bool, std::string, SoftwareChallenge::IndexType, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> load(const std::string& filename);

/**
* @brief distance or number of ties, if any, between two members of that social network
* @param A one member
* @param B another one
* @param name2index mapping.
* @param friendGraph information.
* @return true if exists one and its number of ties, otherwise false. As well some hint about the result.
*/
std::tuple<bool, std::string, SoftwareChallenge::IndexType> searchFriends(const std::string& A, const std::string& B, const SoftwareChallenge::NameIndex& name2index, const SoftwareChallenge::FriendGraph& friendGraph);

/**
* @brief distance or number of ties, if any, between two indexes of that social network
* @param A one member
* @param B another one
* @param name2index mapping.
* @param friendGraph information.
* @return true if exists one and its number of ties, otherwise false. As well some hint about the result.
*/
std::tuple<bool, std::string, SoftwareChallenge::IndexType> searchIndexes(const SoftwareChallenge::IndexType& A, const SoftwareChallenge::IndexType& B, const SoftwareChallenge::NameIndex& name2index, const SoftwareChallenge::FriendGraph& friendGraph);

namespace SoftwareChallenge {

/**
* @brief keep the visited member log in a way that several threads can cooperate
*/
typedef std::vector<std::atomic<IndexType>> Visited;

/**
* @class TiesBFS
* @brief Bare-bones C++ functor in order not to pass too many arguments to its main Breadth First Search method.
*/
class TiesBFS {

  /**
   * @brief flag to exit the thread
   */
  std::atomic<bool>& myDone;

  /**
   * @brief flag to exit of the other thread
   */
  std::atomic<bool>& othersDone;

  /**
   * @brief target to search for
   */
  const IndexType target {0}; // invalid value

  /**
  * @brief graph information of that social network
  */
  const FriendGraph& graph;

  /**
  * @brief visited log in order to avoid cycles in our search
  */
  Visited& myVisited;

  /**
  * @brief The other thread should be doing the same
  */
  Visited& othersVisited;

  /**
   * @bried start point of our search.
   */
  const IndexType start {0};

  /**
   * @brief threadId for debugging
   */
  const std::string threadId;

  /**
   * @brief distance walked until finding some visited from the other thread
   */
  IndexType distance {INDEX_MAX}; // invalid value

  /**
   * @brief common friend, just for debugging
   */
  IndexType common {INDEX_MAX}; // invalid value

public:

  /**
   * @brief Bind external information through its constructor
   * @param done flag to stop this thread.
   * @param otherDone flag to stop other thread.
   * @param targetMember to look for.
   * @param friendGraph social network info.
   * @param mine keeps a log of already processes friends
   * @param others keeps a log of already processes friends
   * @param startPoint to kick off the search
   * @param threadId for debugging
   */
  explicit TiesBFS(std::atomic<bool>& done, std::atomic<bool>& otherDone, const IndexType targetMember, const FriendGraph& friendGraph, Visited& mine, Visited& others, const IndexType startPoint, const std::string& threadId);

  /**
   * @brief distance walked by this thread
   */
  IndexType getDistance() const;

  /**
   * @brief Common friend
   */
  IndexType getCommon() const;

  /**
   * @brief Typical Breadth First Search or BFS for a Graph in order to figure out the number of ties between two members if any.
  */
  void operator()();
};

} // namespace
