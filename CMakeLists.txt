cmake_minimum_required(VERSION 2.8)
project(select_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SOURCES_SERVER "${CMAKE_SOURCE_DIR}/src/select/*.cpp")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_library(${PROJECT_NAME} SHARED ${SOURCES_SERVER})
link_libraries(${PROJECT_NAME})

file(GLOB SOURCES_MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(${PROJECT_NAME}_node ${SOURCES_MAIN})

target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME})
