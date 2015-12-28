#Last Update: 21.01.2015
#author: Julien Wulf
#University: UZH
#E-Mail: jwulf@physik.uzh.ch
#Description:
#The macros to find the packages (OpenCL,Root, etc) are in the "Macro" folder.
#Every Folder contains a CMakeLists.txt, which defines the linking etc
#For Every Subsystem a Libary will be created 


#General Stuff (Paths)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_AUTOMOC OFF)
set (CMAKE_C_COMPILER gcc)
set (CMAKE_CXX_COMPILER g++)

project(DRS_DAQ)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Macro" ${CMAKE_MODULE_PATH})
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_VERBOSE_MAKEFILE NO) 

#All headers of the subsystems muss be placed here 
set (SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set (DRS_DAQ_INCLUDE_DIRS
  ${SOURCE}/Core/Include
)

#Some options for ccmake 
OPTION (XSLAP_VERBOSE "Verbose of the compiler output" OFF)

if(XSLAP_VERBOSE)
	set(CMAKE_VERBOSE_MAKEFILE YES) 
endif(XSLAP_VERBOSE)


#Find external libaries and headers
find_package(ROOT REQUIRED)
find_package(libusb-1.0 REQUIRED)

#if (XSLAP_USE_MPI)
#	find_package(MPI)
 #       set (CMAKE_EXECUTABLE_SUFFIX _mpi) 	
#	add_definitions (-DXSLAP_MPI)
#endif(XSLAP_USE_MPI)

SET(GCC_COVERAGE_COMPILE_FLAGS -DHAVE_USB)
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
add_definitions(-DHAVE_LIBUSB10)
add_definitions(-DLINUX)
add_definitions(-DOS_LINUX)

#set(CMAKE_CXX_FLAGS )

#Global Headers
include_directories(${DRS_DAQ_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${LIBUSB_1_INCLUDE_DIRS})

add_subdirectory(Application)
add_subdirectory(Core)





