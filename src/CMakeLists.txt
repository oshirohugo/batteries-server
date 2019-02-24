cmake_minimum_required (VERSION 2.6)
project (server)

find_package (Threads)

file (GLOB LIB_FILES
  ${CMAKE_SOURCE_DIR}/lib/*.cpp
  ${CMAKE_SOURCE_DIR}/include/*.hpp
)

file (GLOB APP_FILES
  *.cpp
)


add_executable(server ${APP_FILES} ${LIB_FILES})

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT} batteries_lib jsoncpp)

target_include_directories(server PUBLIC ${CMAKE_SOURCE_DIR}/include /opt/boost_1_69_0)