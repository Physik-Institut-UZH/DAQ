project(Core)

set (CORE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/strlcpy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/averager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/DRS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/mxml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/keyb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xmlParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/xml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/DAQContainer.h
)

set (CORE_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/strlcpy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/averager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DRS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/mxml.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/musbstd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/keyb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xmlParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/xml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DAQContainer.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Include ${DRS_DAQ_INCLUDE_DIRS} )
add_library(Core SHARED ${CORE_SOURCEFILES} ${CORE_HEADERFILES})
target_link_libraries(Core ${LIBUSB_1_LIBRARIES} ${ROOT_LIBRARIES})

