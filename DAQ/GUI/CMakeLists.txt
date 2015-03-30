#Subsystem of the Processor

project(GUI)

set (GUI_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/graph.h
)

set (GUI_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/graph.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(GUI SHARED ${GUI_SOURCEFILES} ${GUI_HEADERFILES})
target_link_libraries(GUI Core Plugins ${CAEN} ${ROOT_LIBRARIES})

