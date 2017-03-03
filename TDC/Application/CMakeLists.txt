project(Application)

add_executable(TDC tdc.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS})
target_link_libraries(TDC Core Plugins IO ${CAEN})



