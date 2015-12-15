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
CMAKE_SOURCE_DIR = /data/Software/DAQ/CAEN_DAQ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/Software/DAQ/CAEN_DAQ/build

# Include any dependencies generated for this target.
include Application/CMakeFiles/DAQ.dir/depend.make

# Include the progress variables for this target.
include Application/CMakeFiles/DAQ.dir/progress.make

# Include the compile flags for this target's objects.
include Application/CMakeFiles/DAQ.dir/flags.make

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o: Application/CMakeFiles/DAQ.dir/flags.make
Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o: ../Application/miniDAX.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /data/Software/DAQ/CAEN_DAQ/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o"
	cd /data/Software/DAQ/CAEN_DAQ/build/Application && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/DAQ.dir/miniDAX.cpp.o -c /data/Software/DAQ/CAEN_DAQ/Application/miniDAX.cpp

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DAQ.dir/miniDAX.cpp.i"
	cd /data/Software/DAQ/CAEN_DAQ/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /data/Software/DAQ/CAEN_DAQ/Application/miniDAX.cpp > CMakeFiles/DAQ.dir/miniDAX.cpp.i

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DAQ.dir/miniDAX.cpp.s"
	cd /data/Software/DAQ/CAEN_DAQ/build/Application && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /data/Software/DAQ/CAEN_DAQ/Application/miniDAX.cpp -o CMakeFiles/DAQ.dir/miniDAX.cpp.s

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.requires:
.PHONY : Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.requires

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.provides: Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.requires
	$(MAKE) -f Application/CMakeFiles/DAQ.dir/build.make Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.provides.build
.PHONY : Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.provides

Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.provides.build: Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o

# Object files for target DAQ
DAQ_OBJECTS = \
"CMakeFiles/DAQ.dir/miniDAX.cpp.o"

# External object files for target DAQ
DAQ_EXTERNAL_OBJECTS =

../bin/DAQ: Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o
../bin/DAQ: Application/CMakeFiles/DAQ.dir/build.make
../bin/DAQ: lib/libCore.a
../bin/DAQ: lib/libGUI.a
../bin/DAQ: lib/libPlugins.a
../bin/DAQ: /usr/lib/libCAENVME.so
../bin/DAQ: /opt/root_5.34/lib/libCore.so
../bin/DAQ: /opt/root_5.34/lib/libCint.so
../bin/DAQ: /opt/root_5.34/lib/libRIO.so
../bin/DAQ: /opt/root_5.34/lib/libNet.so
../bin/DAQ: /opt/root_5.34/lib/libHist.so
../bin/DAQ: /opt/root_5.34/lib/libGraf.so
../bin/DAQ: /opt/root_5.34/lib/libGraf3d.so
../bin/DAQ: /opt/root_5.34/lib/libGpad.so
../bin/DAQ: /opt/root_5.34/lib/libTree.so
../bin/DAQ: /opt/root_5.34/lib/libRint.so
../bin/DAQ: /opt/root_5.34/lib/libPostscript.so
../bin/DAQ: /opt/root_5.34/lib/libMatrix.so
../bin/DAQ: /opt/root_5.34/lib/libPhysics.so
../bin/DAQ: /opt/root_5.34/lib/libMathCore.so
../bin/DAQ: /opt/root_5.34/lib/libThread.so
../bin/DAQ: /usr/lib/x86_64-linux-gnu/libdl.so
../bin/DAQ: lib/libCore.a
../bin/DAQ: lib/libPlugins.a
../bin/DAQ: /usr/lib/libCAENVME.so
../bin/DAQ: /opt/root_5.34/lib/libCore.so
../bin/DAQ: /opt/root_5.34/lib/libCint.so
../bin/DAQ: /opt/root_5.34/lib/libRIO.so
../bin/DAQ: /opt/root_5.34/lib/libNet.so
../bin/DAQ: /opt/root_5.34/lib/libHist.so
../bin/DAQ: /opt/root_5.34/lib/libGraf.so
../bin/DAQ: /opt/root_5.34/lib/libGraf3d.so
../bin/DAQ: /opt/root_5.34/lib/libGpad.so
../bin/DAQ: /opt/root_5.34/lib/libTree.so
../bin/DAQ: /opt/root_5.34/lib/libRint.so
../bin/DAQ: /opt/root_5.34/lib/libPostscript.so
../bin/DAQ: /opt/root_5.34/lib/libMatrix.so
../bin/DAQ: /opt/root_5.34/lib/libPhysics.so
../bin/DAQ: /opt/root_5.34/lib/libMathCore.so
../bin/DAQ: /opt/root_5.34/lib/libThread.so
../bin/DAQ: /usr/lib/x86_64-linux-gnu/libdl.so
../bin/DAQ: Application/CMakeFiles/DAQ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/DAQ"
	cd /data/Software/DAQ/CAEN_DAQ/build/Application && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DAQ.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Application/CMakeFiles/DAQ.dir/build: ../bin/DAQ
.PHONY : Application/CMakeFiles/DAQ.dir/build

Application/CMakeFiles/DAQ.dir/requires: Application/CMakeFiles/DAQ.dir/miniDAX.cpp.o.requires
.PHONY : Application/CMakeFiles/DAQ.dir/requires

Application/CMakeFiles/DAQ.dir/clean:
	cd /data/Software/DAQ/CAEN_DAQ/build/Application && $(CMAKE_COMMAND) -P CMakeFiles/DAQ.dir/cmake_clean.cmake
.PHONY : Application/CMakeFiles/DAQ.dir/clean

Application/CMakeFiles/DAQ.dir/depend:
	cd /data/Software/DAQ/CAEN_DAQ/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/Software/DAQ/CAEN_DAQ /data/Software/DAQ/CAEN_DAQ/Application /data/Software/DAQ/CAEN_DAQ/build /data/Software/DAQ/CAEN_DAQ/build/Application /data/Software/DAQ/CAEN_DAQ/build/Application/CMakeFiles/DAQ.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Application/CMakeFiles/DAQ.dir/depend

