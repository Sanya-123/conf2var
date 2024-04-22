cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME conf2var)


project(
  ${PROJECT_NAME}
  VERSION 0.1
  LANGUAGES C CXX
)

# set(EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_CREATE_STATIC_LIBRARY OFF)
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)


include(CheckSymbolExists)
add_subdirectory(libconfig/lib/)


add_library(${PROJECT_NAME} STATIC
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




