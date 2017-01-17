cmake_minimum_required(VERSION 2.6)
project(fontconfigtests)

add_executable(fontconfigtests main.cpp)

install(TARGETS fontconfigtests RUNTIME DESTINATION bin)
