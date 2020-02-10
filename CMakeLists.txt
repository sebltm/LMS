cmake_minimum_required(VERSION 3.15)
project(LMS CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/IO)

add_executable(LMS main.cpp AudioAnalysis.cpp)

target_include_directories(LMS PRIVATE lib/IO/)
target_include_directories(LMS PRIVATE /usr/local/include/eigen3/)
target_include_directories(LMS PRIVATE /usr/local/include/essentia/)
target_include_directories(LMS PRIVATE /usr/local/include/essentia/scheduler/)
target_include_directories(LMS PRIVATE /usr/local/include/essentia/streaming/)
target_include_directories(LMS PRIVATE /usr/local/include/essentia/utils/)

target_link_libraries(LMS portaudio)
target_link_libraries(LMS IO)
target_link_libraries(LMS essentia)
target_link_libraries(LMS fftw3)
target_link_libraries(LMS yaml)
target_link_libraries(LMS avcodec)
target_link_libraries(LMS avformat)
target_link_libraries(LMS avresample)
target_link_libraries(LMS avutil)
target_link_libraries(LMS samplerate)
target_link_libraries(LMS tag)
target_link_libraries(LMS fftw3f)
target_link_libraries(LMS chromaprint)

install(TARGETS LMS DESTINATION bin)
