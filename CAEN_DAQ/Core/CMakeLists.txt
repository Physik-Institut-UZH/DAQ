#Subsystem of the Processor

project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/adc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/control.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xml.h  
)

set (CORE_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/adc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/control.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xml.cpp 
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Core STATIC ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core Plugins ${CAEN} ${ROOT_LIBRARIES})

