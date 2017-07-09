#Subsystem of the Scaler

project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/VMEManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ADCManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/DCManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ADCManager1730.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ScopeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/StorageManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/SlowcontrolManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ADCManager1720.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/ADCManager1724.h
)

set (CORE_SOURCEFILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/VMEManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ADCManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DCManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ADCManager1730.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ScopeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/StorageManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/SlowcontrolManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ADCManager1720.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/ADCManager1724.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${ROOT_INCLUDE_DIRS})
add_library(Core STATIC ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core Plugins ${CAEN})

