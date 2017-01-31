cmake_minimum_required(VERSION 2.6)
project(fontconfigtests)

add_executable(fontconfigtests main.c matrix.c range.c weight.c)

target_link_libraries(fontconfigtests check fontconfig)

install(TARGETS fontconfigtests RUNTIME DESTINATION bin)
