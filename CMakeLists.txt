cmake_minimum_required(VERSION 3.10)
project(sim_library VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# SIM Library
add_library(sim SHARED src/sim.cpp)
target_include_directories(sim PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(sim rt pthread)

# Installation
include(GNUInstallDirs)
install(TARGETS sim
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Examples (optional)
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_executable(sim_writer examples/writer.cpp)
    target_link_libraries(sim_writer sim)
    
    add_executable(sim_reader examples/reader.cpp)
    target_link_libraries(sim_reader sim)
endif()
