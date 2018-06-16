/**
* @file data_structue.hpp
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
#include <vector>

namespace SoftwareChallenge {

	/** @brief Supposed that size_t is too big for indexes that avoid using full string names */
	typedef uint32_t IndexType; // sizeof(size_t) == 2*sizeof(IndexType)

	/** @brief after having processed SocialNetwork.txt, the greatest number implied was 82168, so we'd better double-check it */
	constexpr IndexType INDEX_MAX{ std::numeric_limits<IndexType>::max() }; // used as well as pseudo invalid nullptr
	static_assert(INDEX_MAX > 90000);
	
	/** @brief Knowing that the longest name in our social network is about 24 characters, define its compact version */
	constexpr uint8_t NAME_MAX{ 32 };
	typedef uint8_t NameType[NAME_MAX];
	
	/** @brief double check that a byte is equivalent to a char */
	static_assert(sizeof(char) == sizeof(uint8_t));

	/**
	* class Member
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
	* class NameIndex 
	* @brief Compact info for mapping names and indexes
	*/
	struct NameIndex : public std::map<NameType, IndexType> {
	};

	/**
	* class IndexName
	* @brief Compact info for mapping indexes and names
	*/
	struct IndexName : public std::vector<NameType> {
	};

} // namespace