cmake_minimum_required(VERSION 4.5 FATAL_ERROR)

set(CMAKE_CXX_MODULE_STD ON)

project(CPPasteConsumer LANGUAGES CXX)

include(CTest)
find_package(cppaste CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE synodic::cppaste)

add_test(NAME consumer COMMAND consumer)
