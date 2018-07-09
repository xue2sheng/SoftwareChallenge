/**
* @file data_structure_member.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Compact computer-friendly data structures. Helper to process input text info
*
* The main idea is to work with more compact data structures in order to
* save some space or some processing time in the future.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <tuple>

#include "data_structure_basic.hpp"

namespace SoftwareChallenge {
	
/**
* @brief paif of member to process
*/
typedef std::vector<std::pair<std::string, std::string>> CoupleList;

/**
* @class Member
* @brief Non-compact info for a member of the social network
*/
struct Member : public std::unordered_set<IndexType> {


    /** @brief Index for that member */
    IndexType index{ INDEX_MAX };

   /**
    * @brief Add a new friend
    * @param new_friend add a new friend
    */
   void add(const IndexType& new_friend);
};

} // namespace
