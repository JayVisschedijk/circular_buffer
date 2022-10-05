# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (libcircular_buffer circular_buffer.c)

set(THREADS_PREFER_PTHREAD_FLAG ON) # Without this flag CMake may resort to just '-lpthread'
find_package(Threads)
target_link_libraries(libcircular_buffer Threads::Threads)

find_library(CHECK NAMES check)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (libcircular_buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)