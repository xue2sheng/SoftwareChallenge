/**
* @file file.c
* @author Andres Sevillano 
* @date June 2018
* @brief Thin C wrapper in order for us to invoke C++ code from Go, Rust, Python, .. or similar C-friendly languages.
*
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md
*/

#include "file.h"
#include "file.hpp"

int process_input_file(const char* file_name, unsigned long ** compacted_list, unsigned long * compacted_list_size)
{
	SoftwareChallenge::Collection collection{};

	return 0;
}
