cmake_minimum_required(VERSION 3.15)

project(quikmcasset)

set(EXEC quikmcasset)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc include/)

file(GLOB SOURCES src/*.cpp)

if(CMAKE_CXX_COMPILER_ID EQUAL MSVC)
set(CMAKE_EXE_LINKER_FLAGS /MT)
elseif(CMAKE_CXX_COMPILER_ID EQUAL GNU)
set(CMAKE_EXE_LINKER_FLAGS -static)
endif()

add_executable(${EXEC} ${SOURCES})
