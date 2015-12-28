#Subsystem of the Project

project(IO)

set (IO_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/IOManager.h
)

set (IO_SOURCEFILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/IOManager.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(IO STATIC ${IO_SOURCEFILES} ${IO_HEADERFILES})
target_link_libraries(IO Plugins ${ROOT_LIBRARIES})

