#Last Update: 19.11.2014
#author: Julien Wulf
#University: UZH
#E-Mail: jwulf@physik.uzh.ch
#Description:
#The macros to find the packages (OpenCL,Root, etc) are in the "Macro" folder.
#Every Folder contains a CMakeLists.xt, which defines the linking etc
#For Every Subsystem a Libary will be created 

#General Stuff (Paths)
cmake_minimum_required(VERSION 2.8)

project(Scaler)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Macro" ${CMAKE_MODULE_PATH})
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_VERBOSE_MAKEFILE OFF) 

#All headers of the subsystems muss be placed here 
set (SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set (DAQ_INCLUDE_DIRS
  ${SOURCE}/Core/Include
  ${SOURCE}/Plugins/Include
)

#Find external libaries and headers
find_package(ROOT REQUIRED)
find_library(CAEN NAMES CAENVME PATHS /opt/CAENVME/lib  )

#Compiler Flags
set(CMAKE_CXX_FLAGS -DLINUX)
add_definitions("-std=c++0x") 

#Global Headers
include_directories(${DAQ_INCLUDE_DIRS} /opt/CAENVME/include/)

#Subdirectories
add_subdirectory(Core)
add_subdirectory(Application)
add_subdirectory(Plugins)

