/**
* @file data_structure_graph.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Compact computer-friendly data structures. Focus on friend graphs.
*
* The main idea is to work with more compact data structures in order to
* save some space or some processing time in the future.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <map>
#include <cstdint>
#include <vector>

#include "data_structure_basic.hpp"

namespace SoftwareChallenge {

/**
* @class FriendGraph
* @brief Keep friend relationship in a compact way
*
* Using only indexes makes it easier to serialize/deserialize from/to hard disk/memory.
*
* A vector of vectors of indexes should suffice. Don't forget to adjust their capacity.
*/
struct FriendGraph : public std::vector<std::vector<IndexType>> {

   /**
    * @brief are we friends?
    */
   bool areFriends(const IndexType A, const IndexType B) const;

   /**
   * @brief Raw size
   * @return Number of bytes required for this array
   */
   size_t neededBytes() const;

   /**
    * @brief comparison for tests/debug
    * @param other to compare to
    * @return true if equal
    */
   bool operator ==(const FriendGraph& other) const;

   /**
   * @brief serialize useful info into its raw bytes
   * @return Info in raw bytes
   *
   * if n = number of members and
   *  m = number of friends of then
   * the first n size_t positions will point to
   * each of friends vector
   *
   * 0) one IndexTyep (4 bytes) with the number of members (n)
   * 1) n size_t (8 bytes each) for pointing inside this uint8_t* to
   * 2) n set of IndexType following the structure:
   *   2a) one IndexType with the number of friends for this member (m)
   *   2b) m IndexType as friends indexes for this member
   */
   std::vector<uint8_t> compact() const;

   /**
    * @brief deserialize raw bytes into runnable array
    * @param raw input bytes
    * @return number of members.
   */
   IndexType load(std::vector<uint8_t>& raw);

};

} // namespace

