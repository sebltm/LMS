cmake_minimum_required(VERSION 3.15)
project(IO CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(IO STATIC library.cpp)

target_link_libraries(IO PUBLIC portaudio)
target_link_libraries(IO PRIVATE /usr/lib/x86_64-linux-gnu/libasound.so)
target_link_libraries(IO PRIVATE /usr/lib/x86_64-linux-gnu/libjack.so)

install(TARGETS IO DESTINATION lib)
install(FILES library.h DESTINATION include)
