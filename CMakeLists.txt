cmake_minimum_required(VERSION 3.15)
project(LMS CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(LMS main.cpp FeatureExtraction.cpp FeatureExtraction.h)

target_link_libraries(LMS portaudio)
target_link_libraries(LMS IO)
target_link_libraries(LMS fftw3)

install(TARGETS LMS DESTINATION bin)
