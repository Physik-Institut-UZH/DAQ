#Subsystem of CDAQ
#Author: Julien Wulf UZH

project(Plugins)

set (PLUGINS_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/keyb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xmlParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/common.h
)

set (PLUGINS_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/keyb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xmlParser.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_library(Plugins STATIC ${PLUGINS_SOURCEFILES} ${PLUGINS_HEADERFILES})


