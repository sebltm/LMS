cmake_minimum_required(VERSION 3.15)
project(Gist CXX)

set(CMAKE_CXX_STANDARD 14)

file(GLOB GIST_CORE src/core/*.cpp *.h)
file(GLOB GIST_FFT src/fft/ Window*.cpp Window*.h)
file(GLOB GIST_MFCC src/mfcc/ *.cpp)
file(GLOB GIST_DETECTION src/onset-detection-functions/ *.cpp *.h)
file(GLOB GIST_PITCH src/pitch *.cpp *.h)
file(GLOB GIST src *.cpp *.h)

add_library(Gist STATIC ${GIST_CORE} ${GIST_FFT} ${GIST_MFCC} ${GIST_DETECTION} ${GIST_PITCH} ${GIST})