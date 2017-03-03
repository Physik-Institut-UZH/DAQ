#Subsystem of the Scaler

project(Plugins)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/keyb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xmlParser.h
)

set (CORE_SOURCEFILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/keyb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xmlParser.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Plugins STATIC ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Plugins IO ${CAEN})

