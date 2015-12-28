#Subsystem of the Scaler

project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/VMEManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/DiscriminatorManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ScalerManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xml.h
)

set (CORE_SOURCEFILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/VMEManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DiscriminatorManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ScalerManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xml.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Core STATIC ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core IO Plugins ${CAEN})

