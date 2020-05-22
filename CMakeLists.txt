cmake_minimum_required(VERSION 3.13)
project(Modern)

set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT "C:/Users/Amy/CLionProjects/boost_1_72_0")

find_package(Boost 1.72.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()


add_executable(Modern main.cpp Enigma.cpp ModernCracker.cpp ModernCracker.h Enigma.h)

target_link_libraries (Modern ${Boost_LIBRARIES})