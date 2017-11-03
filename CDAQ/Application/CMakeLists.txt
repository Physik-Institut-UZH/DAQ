project(Application)

add_executable(SingleDAQ SingleDAQ.cpp)
add_executable(MultiDAQ MultiDAQ.cpp) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(SingleDAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(MultiDAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES} ${Boost_LIBRARIES})


