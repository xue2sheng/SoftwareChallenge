/**
* @file data_structure_name2index.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Compact computer-friendly data structures. Focus on string names relationship with indexes.
*
* The main idea is to work with more compact data structures in order to
* save some space or some processing time in the future.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <tuple>
#include <vector>

#include "data_structure_basic.hpp"

namespace SoftwareChallenge {

/**
* @class NameIndex
* @brief Compact info for mapping names and indexes
*
*  NameType is big enough for this social network memeber size < 32 ASCII chars.
*/
struct NameIndex : public std::map<std::string, IndexType> {

#if 0
    /**
     * @brief comparison to debug/test
     * @param other item to compare to
     * @return true if equal
     */
    bool operator==(const NameIndex& other) const;
#endif


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
     * @brief Name info
     * @return Get name if index exists
     */
    std::tuple<bool,std::string> getName(const IndexType index) const;

    /**
    * @brief serialize useful info into its raw bytes
    * @return Info in raw bytes
    */
    std::vector<uint8_t> compact() const;

    /**
    * @brief deserialize raw bytes into runable map
    * @param raw input bytes
    * @return Number of members. It should match map size at the end of loading
    */
    IndexType load(std::vector<uint8_t>& raw);
};

} // namespace


