cmake_minimum_required (VERSION 2.6)
project (tests)


include_directories (${CMAKE_SOURCE_DIR}/include) 

add_executable(player-test player_test.cpp)

target_link_libraries (player-test batteries_lib jsoncpp)