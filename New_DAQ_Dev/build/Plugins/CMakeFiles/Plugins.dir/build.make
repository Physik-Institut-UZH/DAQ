# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/Software/DAQ/New_DAQ_Dev

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/Software/DAQ/New_DAQ_Dev/build

# Include any dependencies generated for this target.
include Plugins/CMakeFiles/Plugins.dir/depend.make

# Include the progress variables for this target.
include Plugins/CMakeFiles/Plugins.dir/progress.make

# Include the compile flags for this target's objects.
include Plugins/CMakeFiles/Plugins.dir/flags.make

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o: Plugins/CMakeFiles/Plugins.dir/flags.make
Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o: ../Plugins/Source/common.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /data/Software/DAQ/New_DAQ_Dev/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Plugins.dir/Source/common.cpp.o -c /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/common.cpp

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Plugins.dir/Source/common.cpp.i"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/common.cpp > CMakeFiles/Plugins.dir/Source/common.cpp.i

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Plugins.dir/Source/common.cpp.s"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/common.cpp -o CMakeFiles/Plugins.dir/Source/common.cpp.s

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.requires:
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.requires

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.provides: Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.requires
	$(MAKE) -f Plugins/CMakeFiles/Plugins.dir/build.make Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.provides.build
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.provides

Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.provides.build: Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o: Plugins/CMakeFiles/Plugins.dir/flags.make
Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o: ../Plugins/Source/keyb.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /data/Software/DAQ/New_DAQ_Dev/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Plugins.dir/Source/keyb.cpp.o -c /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/keyb.cpp

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Plugins.dir/Source/keyb.cpp.i"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/keyb.cpp > CMakeFiles/Plugins.dir/Source/keyb.cpp.i

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Plugins.dir/Source/keyb.cpp.s"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/keyb.cpp -o CMakeFiles/Plugins.dir/Source/keyb.cpp.s

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.requires:
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.requires

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.provides: Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.requires
	$(MAKE) -f Plugins/CMakeFiles/Plugins.dir/build.make Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.provides.build
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.provides

Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.provides.build: Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o: Plugins/CMakeFiles/Plugins.dir/flags.make
Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o: ../Plugins/Source/xmlParser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /data/Software/DAQ/New_DAQ_Dev/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o -c /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/xmlParser.cpp

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Plugins.dir/Source/xmlParser.cpp.i"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/xmlParser.cpp > CMakeFiles/Plugins.dir/Source/xmlParser.cpp.i

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Plugins.dir/Source/xmlParser.cpp.s"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /data/Software/DAQ/New_DAQ_Dev/Plugins/Source/xmlParser.cpp -o CMakeFiles/Plugins.dir/Source/xmlParser.cpp.s

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.requires:
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.requires

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.provides: Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.requires
	$(MAKE) -f Plugins/CMakeFiles/Plugins.dir/build.make Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.provides.build
.PHONY : Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.provides

Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.provides.build: Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o

# Object files for target Plugins
Plugins_OBJECTS = \
"CMakeFiles/Plugins.dir/Source/common.cpp.o" \
"CMakeFiles/Plugins.dir/Source/keyb.cpp.o" \
"CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o"

# External object files for target Plugins
Plugins_EXTERNAL_OBJECTS =

lib/libPlugins.a: Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o
lib/libPlugins.a: Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o
lib/libPlugins.a: Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o
lib/libPlugins.a: Plugins/CMakeFiles/Plugins.dir/build.make
lib/libPlugins.a: Plugins/CMakeFiles/Plugins.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../lib/libPlugins.a"
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && $(CMAKE_COMMAND) -P CMakeFiles/Plugins.dir/cmake_clean_target.cmake
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Plugins.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Plugins/CMakeFiles/Plugins.dir/build: lib/libPlugins.a
.PHONY : Plugins/CMakeFiles/Plugins.dir/build

Plugins/CMakeFiles/Plugins.dir/requires: Plugins/CMakeFiles/Plugins.dir/Source/common.cpp.o.requires
Plugins/CMakeFiles/Plugins.dir/requires: Plugins/CMakeFiles/Plugins.dir/Source/keyb.cpp.o.requires
Plugins/CMakeFiles/Plugins.dir/requires: Plugins/CMakeFiles/Plugins.dir/Source/xmlParser.cpp.o.requires
.PHONY : Plugins/CMakeFiles/Plugins.dir/requires

Plugins/CMakeFiles/Plugins.dir/clean:
	cd /data/Software/DAQ/New_DAQ_Dev/build/Plugins && $(CMAKE_COMMAND) -P CMakeFiles/Plugins.dir/cmake_clean.cmake
.PHONY : Plugins/CMakeFiles/Plugins.dir/clean

Plugins/CMakeFiles/Plugins.dir/depend:
	cd /data/Software/DAQ/New_DAQ_Dev/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/Software/DAQ/New_DAQ_Dev /data/Software/DAQ/New_DAQ_Dev/Plugins /data/Software/DAQ/New_DAQ_Dev/build /data/Software/DAQ/New_DAQ_Dev/build/Plugins /data/Software/DAQ/New_DAQ_Dev/build/Plugins/CMakeFiles/Plugins.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Plugins/CMakeFiles/Plugins.dir/depend

