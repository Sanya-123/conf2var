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

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

set_target_properties(config PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

if (WIN32)
    install(TARGETS conf2var DESTINATION ${CMAKE_BINARY_DIR}/lib)
    add_custom_target(copy_config_libs DEPENDS config config++
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libconfig/lib/*.dll ${CMAKE_BINARY_DIR}/lib/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libconfig/lib/*.dll.a ${CMAKE_BINARY_DIR}/lib/
    )
    add_dependencies(conf2var copy_config_libs)
endif()




