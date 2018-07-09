# CMakeList.txt : CMake project for SoftwareChallenge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)

    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)

    # Add a new target for possible regeneration remotely on the server 
    if(UNIX OR APPLE) # Linux or macOS

	    set(cmake_cache ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	    set(doc_dir ${CMAKE_BINARY_DIR}/doc)
	    set(html_dir ${CMAKE_BINARY_DIR}/html)
	    
	    add_custom_target(regenerate_doc
		    COMMAND ${CMAKE_COMMAND} -E remove -f ${cmake_cache}
		    COMMAND ${CMAKE_COMMAND} -E remove_directory ${doc_dir}
		    COMMAND ${CMAKE_COMMAND} -E remove_directory ${html_dir}
		    COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -DEXTRA:STRING=yes
		    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		    COMMENT "Regenerating API documentation with Doxygen"
		    VERBATIM)

    endif() # Linux or macOS

endif()
