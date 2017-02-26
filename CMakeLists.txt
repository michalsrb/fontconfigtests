cmake_minimum_required(VERSION 2.6)
project(fontconfigtests)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(
    fontconfigtests

    tests/main.c
    tests/match.c
    tests/matrix.c
    tests/range.c
    tests/weight.c
)

target_link_libraries(fontconfigtests check fontconfig)

add_executable(
    fontconfigbenchmark
    benchmark/main.cpp
)

target_link_libraries(fontconfigbenchmark benchmark fontconfig)

# install(TARGETS fontconfigtests RUNTIME DESTINATION bin)
