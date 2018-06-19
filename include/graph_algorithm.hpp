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
* @param file_name to load 
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
  * @brief graph information of that social network
  */
  const FriendGraph& graph;

  /**
  * @brief visited log in order to avoid cycles in our search
  */
  Visited& visited;

  /**
   * @bried start point of our search.
   */
  const IndexType start {0};

public:

  /**
   * @brief Bind external information through its constructor
   * @param friendGRraph social network info.
   * @param visitedMembers keeps a log of already processes friends
   * @param startPoint to kick off the search
   */
  explicit TiesBFS(const FriendGraph& friendGraph, Visited& visitedMembers, const IndexType startPoint);

  /**
   * @brief Typical Breadth First Search or BFS for a Graph in order to figure out the number of ties between two members if any.
   * @param start with a member that search.
  */
  void operator()();
};

} // namespace
