/**
* @file preprocess.hpp
* @author Andres Sevillano 
* @date June 2018
* @brief Process human-friendly input into a more computer-friendly one.
*
* The main idea is to obtain a more compact input in order to
* save some space or some processing time in the future.
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include "data_structure.hpp"

/**
* @brief Get a more compact social network information
* @param file_nane human-friendly input file name
* @return false if error and its description, otherwise true and a summary report. As well, number of members in that social network
*/
std::tuple<bool, std::string, size_t, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> preprocess(const std::string& file_name);

namespace SoftwareChallenge {

	/**
	* class Collection 
	* @brief Non-compact info for a member of the social network
	*/
	class Collection : public std::map<const std::string, Member> {

        // Some redundant comapct info
        /** @brief Name Index mapping */
        NameIndex name2index {};
        /** @brief friends graph */
        FriendGraph friendGraph{};

		// some stats
        /** @brief Number of relationships in the graph */
        size_t relations {0};
        /** @brief Maximum number of friends for one member */
		Member::size_type friends_max {0};			
		/** @brief Minimum number of friends for one member */
		Member::size_type friends_min {INDEX_MAX};	
		/** @brief Maximum length of member name */
		std::string::size_type name_max {0};		
		/** @brief Minimum length of member name */
		std::string::size_type name_min {INDEX_MAX};
		/** @brief Member with more friends */
		std::string popular_max {""};				
		/** @brief Member with fewer friends */
		std::string popular_min {""};				
	
		/**
		* @brief Add a new member if needed 
		* @param new_member add a new member 
		*/
		iterator add(const std::string& key);

		/**
		* @brief Add a new couple of friends
		* @param new_member add a new member 
		*/
		void add(const std::string& key, const std::string& new_friend);

	public:

		/** @brief Reset to its initial values this collection */
		void reset();

        /** @return Number of relationships in the graph */
        inline size_t relationships() const;
        /** @return Maximum number of friends for one member */
		inline Member::size_type friendsMax() const;  
		/** @return Minimum number of friends for one member */
		inline Member::size_type friendsMin() const;
		/** @return Maximum length of member name */
		inline std::string::size_type nameMax() const;
		/** @return Minimum length of member name */
		inline std::string::size_type nameMin() const; 
		/** @return Member with more friends */
		inline std::string popularMax() const;	
		/** @return Member with fewer friends */
		inline std::string popularMin() const;
        /** @return Name Index mapping */
        inline NameIndex nameIndex() const;
        /** @return Name Index mapping */
        inline FriendGraph graph() const;

		/**
		* @brief Given an input file, feed our list of members 
		* @param file_nane human-friendly input file name
		* @return false if error and its description, otherwise true and a summary report
		*/
		std::tuple<bool, std::string> process(const std::string& file_name);

		/**
		* @brief Given a collection, try to compact its information in a computer-friendly way 
		* @param file_nane human-friendly input file name
		* @return false if error and its description, otherwise true and a summary report. As well, the number of members in that social network and extra useful structures.
		*/
        std::tuple<bool, std::string, size_t, NameIndex, FriendGraph> compact();
	};
	
} // namespace
			
