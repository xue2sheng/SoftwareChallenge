/**
* @file data_structure_basic.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Compact computer-friendly data structures. Only basic ones.
*
* The main idea is to work with more compact data structures in order to
* save some space or some processing time in the future.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#pragma once

#include <limits>
#include <cstdint>

namespace SoftwareChallenge {

/** @brief Supposed that size_t is too big for indexes that avoid using full string names */
typedef uint32_t IndexType; // sizeof(size_t) == 2*sizeof(IndexType)

/** @brief to serialize into uint8_t */
union UIndexType {
    /** @brief raw information as an unique integer **/
    IndexType raw;
    /** @brief split information as several bytes **/
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    } split;
    /** @brief split information into bytes
     * @param output result
     **/
    void compact(uint8_t* output) const;
    /** @brief load information from bytes
     * @param input bytes
     **/
    void load(const uint8_t* input);
};

/** @brief after having processed SocialNetwork.txt, the greatest number implied was 82168, so we'd better double-check it */
constexpr IndexType INDEX_MAX{ std::numeric_limits<IndexType>::max() }; // used as well as pseudo invalid nullptr
static_assert(INDEX_MAX > 90000);
	
/** @brief Knowing that the longest name in our social network is about 24 characters, define its compact version */
constexpr const uint8_t NAME_LENTGH_MAX{ 32 };
/** @brief Treated as a bunch of bytes  */
typedef uint8_t NameType[NAME_LENTGH_MAX];
	
/** @brief double check that a byte is equivalent to a char */
static_assert(sizeof(char) == sizeof(uint8_t));

} // namespace
