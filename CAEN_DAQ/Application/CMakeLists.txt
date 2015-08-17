project(Application)

add_executable(DAQ miniDAX.cpp) 
#add_executable(Scaler scaler.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS})
target_link_libraries(DAQ Core GUI Plugins ${CAEN} ${ROOT_LIBRARIES})
#target_link_libraries(Scaler Core GUI Plugins ${CAEN} ${ROOT_LIBRARIES})



