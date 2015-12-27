project(Application)

add_executable(DAQ DAQ.cpp) 
add_executable(Test test.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS})
target_link_libraries(DAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES})
target_link_libraries(Test Core Plugins ${CAEN} ${ROOT_LIBRARIES})



