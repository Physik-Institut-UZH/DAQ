#Subsystem of the Scaler

project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/scaler.hh
)

set (CORE_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/scaler.cc
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Core STATIC ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core Plugins ${CAEN} ${ROOT_LIBRARIES})

