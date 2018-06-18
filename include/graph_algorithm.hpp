/**
* @file graph_algorithm.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Process computer-friendly info.
*/

#pragma once

#include <tuple>
#include <string>

#include "data_structure.hpp"

/**
* @brief load binary info 
* @param file_name to load 
* @return true if success with extra hints. As well data structures
*/
std::tuple<bool, std::string, SoftwareChallenge::IndexType, SoftwareChallenge::NameIndex, SoftwareChallenge::FriendGraph> load(const std::string& filename);
