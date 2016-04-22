project(Application)

add_executable(Scaler main.cc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DAQ_INCLUDE_DIRS})
target_link_libraries(Scaler Core Plugins ${CAEN} ${ROOT_LIBRARIES})


