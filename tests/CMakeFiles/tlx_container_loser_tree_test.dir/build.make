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
include CMakeFiles/tlx_container_loser_tree_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tlx_container_loser_tree_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tlx_container_loser_tree_test.dir/flags.make

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o: CMakeFiles/tlx_container_loser_tree_test.dir/flags.make
CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o: container/loser_tree_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tlx/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o -c /home/ubuntu/tlx/tests/container/loser_tree_test.cpp

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tlx/tests/container/loser_tree_test.cpp > CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.i

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tlx/tests/container/loser_tree_test.cpp -o CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.s

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.requires:

.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.requires

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.provides: CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.requires
	$(MAKE) -f CMakeFiles/tlx_container_loser_tree_test.dir/build.make CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.provides.build
.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.provides

CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.provides.build: CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o


# Object files for target tlx_container_loser_tree_test
tlx_container_loser_tree_test_OBJECTS = \
"CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o"

# External object files for target tlx_container_loser_tree_test
tlx_container_loser_tree_test_EXTERNAL_OBJECTS =

tlx_container_loser_tree_test: CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o
tlx_container_loser_tree_test: CMakeFiles/tlx_container_loser_tree_test.dir/build.make
tlx_container_loser_tree_test: CMakeFiles/tlx_container_loser_tree_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/tlx/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tlx_container_loser_tree_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tlx_container_loser_tree_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tlx_container_loser_tree_test.dir/build: tlx_container_loser_tree_test

.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/build

CMakeFiles/tlx_container_loser_tree_test.dir/requires: CMakeFiles/tlx_container_loser_tree_test.dir/container/loser_tree_test.o.requires

.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/requires

CMakeFiles/tlx_container_loser_tree_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tlx_container_loser_tree_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/clean

CMakeFiles/tlx_container_loser_tree_test.dir/depend:
	cd /home/ubuntu/tlx/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests /home/ubuntu/tlx/tests/CMakeFiles/tlx_container_loser_tree_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tlx_container_loser_tree_test.dir/depend

