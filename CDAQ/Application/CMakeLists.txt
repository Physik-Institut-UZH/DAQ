project(Application)

add_executable(SingleDAQ SingleDAQ.cpp)
add_executable(MultiDAQ MultiDAQ.cpp) 
add_executable(DCDAQ DC_Measurement.cpp) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS})

target_link_libraries(SingleDAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES})
target_link_libraries(DCDAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES})
target_link_libraries(MultiDAQ Core Plugins ${CAEN} ${ROOT_LIBRARIES})


