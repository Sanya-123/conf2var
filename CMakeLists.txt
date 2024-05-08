cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

set(PROJECT_NAME conf2var)


project(
  ${PROJECT_NAME}
  VERSION 0.1
  LANGUAGES C CXX
)

include(CheckSymbolExists)
add_subdirectory(libconfig/lib/)


add_library(${PROJECT_NAME}
    conf2var.h
    conf2var.c
    varcommon/varcommon.c
    varcommon/varcommon.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    varcommon/
    libconfig/lib/
)
target_link_libraries(${PROJECT_NAME} PRIVATE config)

if (WIN32)
    install(TARGETS conf2var DESTINATION ${CMAKE_BINARY_DIR}/lib)
    install(TARGETS config
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/lib
        OBJECTS DESTINATION ${CMAKE_BINARY_DIR}/lib
    )
    install(TARGETS config++
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/lib
        OBJECTS DESTINATION ${CMAKE_BINARY_DIR}/lib
    )
endif()




