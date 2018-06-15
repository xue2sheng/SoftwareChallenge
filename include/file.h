/**
* @file file.h
* @author Andres Sevillano
* @date June 2018
* @brief Process human-friendly input into a more computer-friendly one.
*
* The main idea is to obtain a more compact input in order to
* save some space or some processing time in the future.
* @see https://github.com/xue2sheng/SoftwareChallenge/blob/master/README.md#approach
*/

#ifndef SOFTWARE_CHALLENGE_FILE
#define SOFTWARE_CHALLENGE_FILE

extern "C" {

	/**
	* @brief Given a human-friendly input file and provides its computer-friendly equivalent
	*/
	int process_input_file(const char* file_name, unsigned long ** compacted_list, unsigned long * compacted_list_size);

} // extern "C"

#endif // SOFTWARE_CHALLENGE_FILE
