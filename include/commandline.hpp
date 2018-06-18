/**
* @file commandline.hpp
* @author Andres Sevillano
* @date June 2018
* @brief Process user commandline arguments.
*/

#pragma once

#include <tuple>
#include <string>

/**
* @brief process user options for running the binary
* @param argc number of arguments
* @param argv arguments info
* @return user options processed
*
* There usually is some library in any organization, based on boost or getops (Unix), but some basic approach from Internet was followed
* @see https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
*/
std::tuple<std::string, bool, std::string, bool, bool> commandline_arguments(int argc, char** argv);
