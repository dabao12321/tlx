# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/tlx/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/tlx/tests

# Include any dependencies generated for this target.
include CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/flags.make

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/flags.make
CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o: algorithm/multiway_merge_benchmark.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tlx/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o -c /home/ubuntu/tlx/tests/algorithm/multiway_merge_benchmark.cpp

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tlx/tests/algorithm/multiway_merge_benchmark.cpp > CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.i

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tlx/tests/algorithm/multiway_merge_benchmark.cpp -o CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.s

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.requires:

.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.requires

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.provides: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.requires
	$(MAKE) -f CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/build.make CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.provides.build
.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.provides

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.provides.build: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o


# Object files for target tlx_algorithm_multiway_merge_benchmark
tlx_algorithm_multiway_merge_benchmark_OBJECTS = \
"CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o"

# External object files for target tlx_algorithm_multiway_merge_benchmark
tlx_algorithm_multiway_merge_benchmark_EXTERNAL_OBJECTS =

tlx_algorithm_multiway_merge_benchmark: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o
tlx_algorithm_multiway_merge_benchmark: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/build.make
tlx_algorithm_multiway_merge_benchmark: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/tlx/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tlx_algorithm_multiway_merge_benchmark"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/build: tlx_algorithm_multiway_merge_benchmark

.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/build

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/requires: CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/algorithm/multiway_merge_benchmark.o.requires

.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/requires

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/clean

CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/depend:
	cd /home/ubuntu/tlx/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests/CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tlx_algorithm_multiway_merge_benchmark.dir/depend
