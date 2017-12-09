cmake_minimum_required(VERSION 2.8)

project(practice)

include_directories(../include)
link_directories(${PROJECT_SOURCE_DIR}/../lib)

AUX_SOURCE_DIRECTORY(./src src_list)
add_executable(practice ${src_list})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
if(UNIX)
add_definitions(-g)
add_definitions(-std=c++11)
TARGET_LINK_LIBRARIES(practice gtest pthread)
endif()

if(WIN32)
TARGET_LINK_LIBRARIES(practice gtestd)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()
