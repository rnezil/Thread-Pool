# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/sde/packages/cmake-3.26.3/bin/cmake

# The command to remove a file.
RM = /opt/sde/packages/cmake-3.26.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jdoe/cpp_concurrency-rnezil

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jdoe/cpp_concurrency-rnezil/tmp

# Include any dependencies generated for this target.
include CMakeFiles/test_thread_pool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_thread_pool.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_thread_pool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_thread_pool.dir/flags.make

CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o: CMakeFiles/test_thread_pool.dir/flags.make
CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o: /home/jdoe/cpp_concurrency-rnezil/app/test_thread_pool.cpp
CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o: CMakeFiles/test_thread_pool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jdoe/cpp_concurrency-rnezil/tmp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o"
	/opt/sde/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o -MF CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o.d -o CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o -c /home/jdoe/cpp_concurrency-rnezil/app/test_thread_pool.cpp

CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.i"
	/opt/sde/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jdoe/cpp_concurrency-rnezil/app/test_thread_pool.cpp > CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.i

CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.s"
	/opt/sde/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jdoe/cpp_concurrency-rnezil/app/test_thread_pool.cpp -o CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.s

# Object files for target test_thread_pool
test_thread_pool_OBJECTS = \
"CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o"

# External object files for target test_thread_pool
test_thread_pool_EXTERNAL_OBJECTS =

test_thread_pool: CMakeFiles/test_thread_pool.dir/app/test_thread_pool.cpp.o
test_thread_pool: CMakeFiles/test_thread_pool.dir/build.make
test_thread_pool: libthread_pool.a
test_thread_pool: CMakeFiles/test_thread_pool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jdoe/cpp_concurrency-rnezil/tmp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_thread_pool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_thread_pool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_thread_pool.dir/build: test_thread_pool
.PHONY : CMakeFiles/test_thread_pool.dir/build

CMakeFiles/test_thread_pool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_thread_pool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_thread_pool.dir/clean

CMakeFiles/test_thread_pool.dir/depend:
	cd /home/jdoe/cpp_concurrency-rnezil/tmp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jdoe/cpp_concurrency-rnezil /home/jdoe/cpp_concurrency-rnezil /home/jdoe/cpp_concurrency-rnezil/tmp /home/jdoe/cpp_concurrency-rnezil/tmp /home/jdoe/cpp_concurrency-rnezil/tmp/CMakeFiles/test_thread_pool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_thread_pool.dir/depend

