# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/dongbo/file/clion/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/dongbo/file/clion/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dongbo/Documents/test_code/kfkclient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dongbo/Documents/test_code/kfkclient/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/kfkclient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kfkclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kfkclient.dir/flags.make

CMakeFiles/kfkclient.dir/test_main2.cpp.o: CMakeFiles/kfkclient.dir/flags.make
CMakeFiles/kfkclient.dir/test_main2.cpp.o: ../test_main2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dongbo/Documents/test_code/kfkclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kfkclient.dir/test_main2.cpp.o"
	/opt/rh/devtoolset-8/root/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kfkclient.dir/test_main2.cpp.o -c /home/dongbo/Documents/test_code/kfkclient/test_main2.cpp

CMakeFiles/kfkclient.dir/test_main2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kfkclient.dir/test_main2.cpp.i"
	/opt/rh/devtoolset-8/root/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dongbo/Documents/test_code/kfkclient/test_main2.cpp > CMakeFiles/kfkclient.dir/test_main2.cpp.i

CMakeFiles/kfkclient.dir/test_main2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kfkclient.dir/test_main2.cpp.s"
	/opt/rh/devtoolset-8/root/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dongbo/Documents/test_code/kfkclient/test_main2.cpp -o CMakeFiles/kfkclient.dir/test_main2.cpp.s

# Object files for target kfkclient
kfkclient_OBJECTS = \
"CMakeFiles/kfkclient.dir/test_main2.cpp.o"

# External object files for target kfkclient
kfkclient_EXTERNAL_OBJECTS =

kfkclient: CMakeFiles/kfkclient.dir/test_main2.cpp.o
kfkclient: CMakeFiles/kfkclient.dir/build.make
kfkclient: libsyrdkafka.a
kfkclient: CMakeFiles/kfkclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dongbo/Documents/test_code/kfkclient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kfkclient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kfkclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kfkclient.dir/build: kfkclient

.PHONY : CMakeFiles/kfkclient.dir/build

CMakeFiles/kfkclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kfkclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kfkclient.dir/clean

CMakeFiles/kfkclient.dir/depend:
	cd /home/dongbo/Documents/test_code/kfkclient/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dongbo/Documents/test_code/kfkclient /home/dongbo/Documents/test_code/kfkclient /home/dongbo/Documents/test_code/kfkclient/cmake-build-debug /home/dongbo/Documents/test_code/kfkclient/cmake-build-debug /home/dongbo/Documents/test_code/kfkclient/cmake-build-debug/CMakeFiles/kfkclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kfkclient.dir/depend

