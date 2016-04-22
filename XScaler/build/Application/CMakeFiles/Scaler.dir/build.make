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
CMAKE_SOURCE_DIR = /home/sandbox/DAQ/XScaler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sandbox/DAQ/XScaler/build

# Include any dependencies generated for this target.
include Application/CMakeFiles/Scaler.dir/depend.make

# Include the progress variables for this target.
include Application/CMakeFiles/Scaler.dir/progress.make

# Include the compile flags for this target's objects.
include Application/CMakeFiles/Scaler.dir/flags.make

Application/CMakeFiles/Scaler.dir/main.cc.o: Application/CMakeFiles/Scaler.dir/flags.make
Application/CMakeFiles/Scaler.dir/main.cc.o: ../Application/main.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/XScaler/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Application/CMakeFiles/Scaler.dir/main.cc.o"
	cd /home/sandbox/DAQ/XScaler/build/Application && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Scaler.dir/main.cc.o -c /home/sandbox/DAQ/XScaler/Application/main.cc

Application/CMakeFiles/Scaler.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scaler.dir/main.cc.i"
	cd /home/sandbox/DAQ/XScaler/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/XScaler/Application/main.cc > CMakeFiles/Scaler.dir/main.cc.i

Application/CMakeFiles/Scaler.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scaler.dir/main.cc.s"
	cd /home/sandbox/DAQ/XScaler/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/XScaler/Application/main.cc -o CMakeFiles/Scaler.dir/main.cc.s

Application/CMakeFiles/Scaler.dir/main.cc.o.requires:
.PHONY : Application/CMakeFiles/Scaler.dir/main.cc.o.requires

Application/CMakeFiles/Scaler.dir/main.cc.o.provides: Application/CMakeFiles/Scaler.dir/main.cc.o.requires
	$(MAKE) -f Application/CMakeFiles/Scaler.dir/build.make Application/CMakeFiles/Scaler.dir/main.cc.o.provides.build
.PHONY : Application/CMakeFiles/Scaler.dir/main.cc.o.provides

Application/CMakeFiles/Scaler.dir/main.cc.o.provides.build: Application/CMakeFiles/Scaler.dir/main.cc.o

# Object files for target Scaler
Scaler_OBJECTS = \
"CMakeFiles/Scaler.dir/main.cc.o"

# External object files for target Scaler
Scaler_EXTERNAL_OBJECTS =

../bin/Scaler: Application/CMakeFiles/Scaler.dir/main.cc.o
../bin/Scaler: Application/CMakeFiles/Scaler.dir/build.make
../bin/Scaler: lib/libCore.a
../bin/Scaler: lib/libPlugins.a
../bin/Scaler: /usr/lib/libCAENVME.so
../bin/Scaler: /opt/root/lib/libCore.so
../bin/Scaler: /opt/root/lib/libRIO.so
../bin/Scaler: /opt/root/lib/libNet.so
../bin/Scaler: /opt/root/lib/libHist.so
../bin/Scaler: /opt/root/lib/libGraf.so
../bin/Scaler: /opt/root/lib/libGraf3d.so
../bin/Scaler: /opt/root/lib/libGpad.so
../bin/Scaler: /opt/root/lib/libTree.so
../bin/Scaler: /opt/root/lib/libRint.so
../bin/Scaler: /opt/root/lib/libPostscript.so
../bin/Scaler: /opt/root/lib/libMatrix.so
../bin/Scaler: /opt/root/lib/libPhysics.so
../bin/Scaler: /opt/root/lib/libMathCore.so
../bin/Scaler: /opt/root/lib/libThread.so
../bin/Scaler: /usr/lib/x86_64-linux-gnu/libdl.so
../bin/Scaler: Application/CMakeFiles/Scaler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/Scaler"
	cd /home/sandbox/DAQ/XScaler/build/Application && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Scaler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Application/CMakeFiles/Scaler.dir/build: ../bin/Scaler
.PHONY : Application/CMakeFiles/Scaler.dir/build

Application/CMakeFiles/Scaler.dir/requires: Application/CMakeFiles/Scaler.dir/main.cc.o.requires
.PHONY : Application/CMakeFiles/Scaler.dir/requires

Application/CMakeFiles/Scaler.dir/clean:
	cd /home/sandbox/DAQ/XScaler/build/Application && $(CMAKE_COMMAND) -P CMakeFiles/Scaler.dir/cmake_clean.cmake
.PHONY : Application/CMakeFiles/Scaler.dir/clean

Application/CMakeFiles/Scaler.dir/depend:
	cd /home/sandbox/DAQ/XScaler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sandbox/DAQ/XScaler /home/sandbox/DAQ/XScaler/Application /home/sandbox/DAQ/XScaler/build /home/sandbox/DAQ/XScaler/build/Application /home/sandbox/DAQ/XScaler/build/Application/CMakeFiles/Scaler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Application/CMakeFiles/Scaler.dir/depend

