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
include Core/CMakeFiles/Core.dir/depend.make

# Include the progress variables for this target.
include Core/CMakeFiles/Core.dir/progress.make

# Include the compile flags for this target's objects.
include Core/CMakeFiles/Core.dir/flags.make

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o: ../Core/Source/VMEManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/VMEManager.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/VMEManager.cpp

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/VMEManager.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/VMEManager.cpp > CMakeFiles/Core.dir/Source/VMEManager.cpp.i

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/VMEManager.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/VMEManager.cpp -o CMakeFiles/Core.dir/Source/VMEManager.cpp.s

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o: ../Core/Source/ADCManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/ADCManager.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager.cpp

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/ADCManager.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager.cpp > CMakeFiles/Core.dir/Source/ADCManager.cpp.i

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/ADCManager.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager.cpp -o CMakeFiles/Core.dir/Source/ADCManager.cpp.s

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o: ../Core/Source/ADCManager1730.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1730.cpp

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/ADCManager1730.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1730.cpp > CMakeFiles/Core.dir/Source/ADCManager1730.cpp.i

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/ADCManager1730.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1730.cpp -o CMakeFiles/Core.dir/Source/ADCManager1730.cpp.s

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o: ../Core/Source/ScopeManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/ScopeManager.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/ScopeManager.cpp

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/ScopeManager.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/ScopeManager.cpp > CMakeFiles/Core.dir/Source/ScopeManager.cpp.i

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/ScopeManager.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/ScopeManager.cpp -o CMakeFiles/Core.dir/Source/ScopeManager.cpp.s

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o: ../Core/Source/StorageManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/StorageManager.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/StorageManager.cpp

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/StorageManager.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/StorageManager.cpp > CMakeFiles/Core.dir/Source/StorageManager.cpp.i

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/StorageManager.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/StorageManager.cpp -o CMakeFiles/Core.dir/Source/StorageManager.cpp.s

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o: ../Core/Source/SlowcontrolManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/SlowcontrolManager.cpp

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/SlowcontrolManager.cpp > CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.i

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/SlowcontrolManager.cpp -o CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.s

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o: ../Core/Source/ADCManager1720.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1720.cpp

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/ADCManager1720.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1720.cpp > CMakeFiles/Core.dir/Source/ADCManager1720.cpp.i

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/ADCManager1720.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1720.cpp -o CMakeFiles/Core.dir/Source/ADCManager1720.cpp.s

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o: Core/CMakeFiles/Core.dir/flags.make
Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o: ../Core/Source/ADCManager1724.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/sandbox/DAQ/CDAQ/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o -c /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1724.cpp

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Core.dir/Source/ADCManager1724.cpp.i"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1724.cpp > CMakeFiles/Core.dir/Source/ADCManager1724.cpp.i

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Core.dir/Source/ADCManager1724.cpp.s"
	cd /home/sandbox/DAQ/CDAQ/build/Core && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/sandbox/DAQ/CDAQ/Core/Source/ADCManager1724.cpp -o CMakeFiles/Core.dir/Source/ADCManager1724.cpp.s

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.requires:
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.requires

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.provides: Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.requires
	$(MAKE) -f Core/CMakeFiles/Core.dir/build.make Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.provides.build
.PHONY : Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.provides

Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.provides.build: Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o

# Object files for target Core
Core_OBJECTS = \
"CMakeFiles/Core.dir/Source/VMEManager.cpp.o" \
"CMakeFiles/Core.dir/Source/ADCManager.cpp.o" \
"CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o" \
"CMakeFiles/Core.dir/Source/ScopeManager.cpp.o" \
"CMakeFiles/Core.dir/Source/StorageManager.cpp.o" \
"CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o" \
"CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o" \
"CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o"

# External object files for target Core
Core_EXTERNAL_OBJECTS =

lib/libCore.a: Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o
lib/libCore.a: Core/CMakeFiles/Core.dir/build.make
lib/libCore.a: Core/CMakeFiles/Core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../lib/libCore.a"
	cd /home/sandbox/DAQ/CDAQ/build/Core && $(CMAKE_COMMAND) -P CMakeFiles/Core.dir/cmake_clean_target.cmake
	cd /home/sandbox/DAQ/CDAQ/build/Core && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Core/CMakeFiles/Core.dir/build: lib/libCore.a
.PHONY : Core/CMakeFiles/Core.dir/build

Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/VMEManager.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/ADCManager.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/ADCManager1730.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/ScopeManager.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/StorageManager.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/SlowcontrolManager.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/ADCManager1720.cpp.o.requires
Core/CMakeFiles/Core.dir/requires: Core/CMakeFiles/Core.dir/Source/ADCManager1724.cpp.o.requires
.PHONY : Core/CMakeFiles/Core.dir/requires

Core/CMakeFiles/Core.dir/clean:
	cd /home/sandbox/DAQ/CDAQ/build/Core && $(CMAKE_COMMAND) -P CMakeFiles/Core.dir/cmake_clean.cmake
.PHONY : Core/CMakeFiles/Core.dir/clean

Core/CMakeFiles/Core.dir/depend:
	cd /home/sandbox/DAQ/CDAQ/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sandbox/DAQ/CDAQ /home/sandbox/DAQ/CDAQ/Core /home/sandbox/DAQ/CDAQ/build /home/sandbox/DAQ/CDAQ/build/Core /home/sandbox/DAQ/CDAQ/build/Core/CMakeFiles/Core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Core/CMakeFiles/Core.dir/depend

