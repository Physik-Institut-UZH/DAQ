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
CMAKE_SOURCE_DIR = /home/sandbox/DAQ/CDAQ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sandbox/DAQ/CDAQ/build

# Include any dependencies generated for this target.
include Application/CMakeFiles/SingleDAQ.dir/depend.make

# Include the progress variables for this target.
include Application/CMakeFiles/SingleDAQ.dir/progress.make

# Include the compile flags for this target's objects.
include Application/CMakeFiles/SingleDAQ.dir/flags.make

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o: Application/CMakeFiles/SingleDAQ.dir/flags.make
Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o: ../Application/SingleDAQ.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Application && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o -c /home/sandbox/DAQ/CDAQ/Application/SingleDAQ.cpp

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Application/SingleDAQ.cpp > CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.i

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Application/SingleDAQ.cpp -o CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.s

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.requires:
.PHONY : Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.requires

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.provides: Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.requires
	$(MAKE) -f Application/CMakeFiles/SingleDAQ.dir/build.make Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.provides.build
.PHONY : Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.provides

Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.provides.build: Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o

# Object files for target SingleDAQ
SingleDAQ_OBJECTS = \
"CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o"

# External object files for target SingleDAQ
SingleDAQ_EXTERNAL_OBJECTS =

../bin/SingleDAQ: Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o
../bin/SingleDAQ: Application/CMakeFiles/SingleDAQ.dir/build.make
../bin/SingleDAQ: lib/libCore.a
../bin/SingleDAQ: lib/libPlugins.a
../bin/SingleDAQ: /usr/lib/libCAENVME.so
../bin/SingleDAQ: /opt/root/lib/libCore.so
../bin/SingleDAQ: /opt/root/lib/libRIO.so
../bin/SingleDAQ: /opt/root/lib/libNet.so
../bin/SingleDAQ: /opt/root/lib/libHist.so
../bin/SingleDAQ: /opt/root/lib/libGraf.so
../bin/SingleDAQ: /opt/root/lib/libGraf3d.so
../bin/SingleDAQ: /opt/root/lib/libGpad.so
../bin/SingleDAQ: /opt/root/lib/libTree.so
../bin/SingleDAQ: /opt/root/lib/libRint.so
../bin/SingleDAQ: /opt/root/lib/libPostscript.so
../bin/SingleDAQ: /opt/root/lib/libMatrix.so
../bin/SingleDAQ: /opt/root/lib/libPhysics.so
../bin/SingleDAQ: /opt/root/lib/libMathCore.so
../bin/SingleDAQ: /opt/root/lib/libThread.so
../bin/SingleDAQ: /usr/lib/x86_64-linux-gnu/libdl.so
../bin/SingleDAQ: Application/CMakeFiles/SingleDAQ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/SingleDAQ"
	cd /home/sandbox/DAQ/CDAQ/build/Application && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SingleDAQ.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Application/CMakeFiles/SingleDAQ.dir/build: ../bin/SingleDAQ
.PHONY : Application/CMakeFiles/SingleDAQ.dir/build

Application/CMakeFiles/SingleDAQ.dir/requires: Application/CMakeFiles/SingleDAQ.dir/SingleDAQ.cpp.o.requires
.PHONY : Application/CMakeFiles/SingleDAQ.dir/requires

Application/CMakeFiles/SingleDAQ.dir/clean:
	cd /home/sandbox/DAQ/CDAQ/build/Application && $(CMAKE_COMMAND) -P CMakeFiles/SingleDAQ.dir/cmake_clean.cmake
.PHONY : Application/CMakeFiles/SingleDAQ.dir/clean

Application/CMakeFiles/SingleDAQ.dir/depend:
	cd /home/sandbox/DAQ/CDAQ/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sandbox/DAQ/CDAQ /home/sandbox/DAQ/CDAQ/Application /home/sandbox/DAQ/CDAQ/build /home/sandbox/DAQ/CDAQ/build/Application /home/sandbox/DAQ/CDAQ/build/Application/CMakeFiles/SingleDAQ.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Application/CMakeFiles/SingleDAQ.dir/depend

