# find_package(Check REQUIRED)

# include(CheckCSourceCompiles)
# include(CheckCSourceRuns)
# include(CheckFunctionExists)
# include(CheckIncludeFile)
# include(CheckIncludeFiles)
# include(CheckLibraryExists)
# include(CheckSymbolExists)
# include(CheckTypeSize)

FIND_PATH( CHECK_INCLUDE_DIR check.h )
FIND_LIBRARY( CHECK_LIBRARIES NAMES check )

# Since Check uses Threads to paralelize the tests, it's mandatory
# add pthread as a dependency, alongside the Check libraries.
add_executable(test_circular_buffer test_circular_buffer.c)
target_link_libraries(test_circular_buffer m libcircular_buffer ${CHECK_LIBRARIES})
target_include_directories (test_circular_buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME test_circular_buffer COMMAND test_circular_buffer WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_circular_buffer PROPERTIES TIMEOUT 30) 
