/**
* @file data_structure_basic.cpp
* @author Andres Sevillano 
* @date June 2018
* @brief Utilities to compact computer-friendly collection info. Just to process binary indexes.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include <cstdlib>
#include <cstring>

#include "data_structure_basic.hpp"

using namespace SoftwareChallenge;

void UIndexType::compact(uint8_t* output) const
{
    // overwrite
    std::memcpy(output + 0*sizeof(uint8_t), &split.a, sizeof(uint8_t));
    std::memcpy(output + 1*sizeof(uint8_t), &split.b, sizeof(uint8_t));
    std::memcpy(output + 2*sizeof(uint8_t), &split.c, sizeof(uint8_t));
    std::memcpy(output + 3*sizeof(uint8_t), &split.d, sizeof(uint8_t));
}

void UIndexType::load(const uint8_t* input)
{
    // clean
    raw = 0;

    // load
    split.a = *(input + 0*sizeof(uint8_t));
    split.b = *(input + 1*sizeof(uint8_t));
    split.c = *(input + 2*sizeof(uint8_t));
    split.d = *(input + 3*sizeof(uint8_t));
}
