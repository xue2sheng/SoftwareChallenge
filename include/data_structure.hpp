/**
* @file data_structure.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Compact computer-friendly data structures.
*
* The main idea is to work with more compact data structures in order to
* save some space or some processing time in the future.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <map>
#include <unordered_set>
#include <limits>
#include <cstdint>
#include <tuple>
#include <vector>

namespace SoftwareChallenge {

/** @brief Supposed that size_t is too big for indexes that avoid using full string names */
typedef uint32_t IndexType; // sizeof(size_t) == 2*sizeof(IndexType)

/** @brief after having processed SocialNetwork.txt, the greatest number implied was 82168, so we'd better double-check it */
constexpr IndexType INDEX_MAX{ std::numeric_limits<IndexType>::max() }; // used as well as pseudo invalid nullptr
static_assert(INDEX_MAX > 90000);
	
/** @brief Knowing that the longest name in our social network is about 24 characters, define its compact version */
constexpr const uint8_t NAME_LENTGH_MAX{ 32 };
typedef uint8_t NameType[NAME_LENTGH_MAX];
	
/** @brief double check that a byte is equivalent to a char */
static_assert(sizeof(char) == sizeof(uint8_t));

/**
* @brief paif of member to process
*/
typedef std::vector<std::pair<std::string, std::string>> CoupleList;

/**
* @class Member
* @brief Non-compact info for a member of the social network
*/
struct Member : public std::unordered_set<IndexType> {

    IndexType index{ INDEX_MAX }; /** Index for that member */

   /**
    * @brief Add a new friend
    * @param new_friend add a new friend
    */
   void add(const IndexType& new_friend);
};

/**
* @class NameIndex
* @brief Compact info for mapping names and indexes
*
*  NameType is big enough for this social network memeber size < 32 ASCII chars.
*/
struct NameIndex : public std::map<std::string, IndexType> {

    /**
    * @brief Raw size
    * @return Number of bytes required for this map
    */
    size_t neededBytes() const;

    /**
    * @brief Index info
    * @return Get index if name exists
    */
    std::tuple<bool,IndexType> getIndex(const std::string& name) const;

    /**
    * @brief serialize useful info into its raw bytes
    * @return Info in raw bytes
    */
    std::vector<uint8_t> compact() const;

    /**
    * @brief deserialize raw bytes into runable map
    * @param raw input bytes
    * @return size stored in the raw data. It should match map size at the end of loading
    */
    IndexType load(std::vector<uint8_t>& raw);
};

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
    * @return size stored in the raw data. It should match map size at the end of loading
   */
   IndexType load(std::vector<uint8_t>& raw);

};

} // namespace
