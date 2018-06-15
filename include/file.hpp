/**
* @file file.hpp
* @author Andres Sevillano 
* @date June 2018
* @brief Process human-friendly input into a more computer-friendly one.
*
* The main idea is to obtain a more compact input in order to
* save some space or some processing time in the future.
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <map>
#include <unordered_set>

namespace SoftwareChallenge {

	/**
	* @brief Supposed that ulong is big enough to be the index for that social network in order to avoid using full names
	*/
	typedef size_t IndexType;

	/**
	* class Member
	* @brief Non-compact info for a member of the social network
	*/
	struct Member : public std::unordered_set<IndexType> {

		IndexType index; /** Index for that member */

		/**
		* @brief Add a new friend 
		* @param new_friend add a new friend
		*/
		void add(const IndexType& new_friend);
	};

	/**
	* class Collection 
	* @brief Non-compact info for a member of the social network
	*/
	struct Collection : public std::map<const std::string, Member> {

		/**
		* @brief Given an input file, feed our list of members 
		* @param file_nane human-friendly input file name
		* @return false if error
		*/
		bool process(const std::string& file_name);

		/**
		* @brief Add a new member 
		* @param new_member add a new member 
		*/
		void add(const std::string& key, const std::string& new_friend);
	
	};

} // namespace
			
