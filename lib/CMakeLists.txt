file(GLOB SRC_FILES *.cpp)
add_library(batteries_lib ${SRC_FILES})

target_include_directories(batteries_lib PUBLIC ${CMAKE_SOURCE_DIR}/include /opt/boost_1_69_0)