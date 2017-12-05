cmake_minimum_required(VERSION 3.9)
project(yazik)

set(CMAKE_CXX_STANDARD 17)

set(srcDir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE atomics ${srcDir}/atomics/*.h ${srcDir}/atomics/*.cpp)
file(GLOB_RECURSE o2tree ${srcDir}/o2tree/*.h ${srcDir}/o2tree/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCE_FILES main.cpp ${atomics} ${o2tree})

add_executable(yazik ${SOURCE_FILES})