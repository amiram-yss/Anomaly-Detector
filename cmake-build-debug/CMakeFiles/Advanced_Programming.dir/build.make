# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/amiram/Desktop/Advanced Programming"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/amiram/Desktop/Advanced Programming/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Advanced_Programming.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Advanced_Programming.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Advanced_Programming.dir/flags.make

CMakeFiles/Advanced_Programming.dir/main.cpp.o: CMakeFiles/Advanced_Programming.dir/flags.make
CMakeFiles/Advanced_Programming.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/amiram/Desktop/Advanced Programming/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Advanced_Programming.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Advanced_Programming.dir/main.cpp.o -c "/home/amiram/Desktop/Advanced Programming/main.cpp"

CMakeFiles/Advanced_Programming.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Advanced_Programming.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/amiram/Desktop/Advanced Programming/main.cpp" > CMakeFiles/Advanced_Programming.dir/main.cpp.i

CMakeFiles/Advanced_Programming.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Advanced_Programming.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/amiram/Desktop/Advanced Programming/main.cpp" -o CMakeFiles/Advanced_Programming.dir/main.cpp.s

# Object files for target Advanced_Programming
Advanced_Programming_OBJECTS = \
"CMakeFiles/Advanced_Programming.dir/main.cpp.o"

# External object files for target Advanced_Programming
Advanced_Programming_EXTERNAL_OBJECTS =

Advanced_Programming: CMakeFiles/Advanced_Programming.dir/main.cpp.o
Advanced_Programming: CMakeFiles/Advanced_Programming.dir/build.make
Advanced_Programming: CMakeFiles/Advanced_Programming.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/amiram/Desktop/Advanced Programming/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Advanced_Programming"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Advanced_Programming.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Advanced_Programming.dir/build: Advanced_Programming
.PHONY : CMakeFiles/Advanced_Programming.dir/build

CMakeFiles/Advanced_Programming.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Advanced_Programming.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Advanced_Programming.dir/clean

CMakeFiles/Advanced_Programming.dir/depend:
	cd "/home/amiram/Desktop/Advanced Programming/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/amiram/Desktop/Advanced Programming" "/home/amiram/Desktop/Advanced Programming" "/home/amiram/Desktop/Advanced Programming/cmake-build-debug" "/home/amiram/Desktop/Advanced Programming/cmake-build-debug" "/home/amiram/Desktop/Advanced Programming/cmake-build-debug/CMakeFiles/Advanced_Programming.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Advanced_Programming.dir/depend

