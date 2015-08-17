#Subsystem of the Scaler

project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/discriminator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/cfgReader.h
)

set (CORE_SOURCEFILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/discriminator.cpp

)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Core SHARED ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core ${CAEN} ${ROOT_LIBRARIES})

