project(Application)
add_executable(ZDAQ ZDAQ.cpp ) 
add_executable(Test test.cpp )


include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${DRS_DAQ_INCLUDE_DIRS})
link_directories(${CMAKE_SOURCE_DIR}/build/lib/)

target_link_libraries(ZDAQ Core ${LIBUSB_1_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(Test Core ${LIBUSB_1_LIBRARIES} ${ROOT_LIBRARIES})



