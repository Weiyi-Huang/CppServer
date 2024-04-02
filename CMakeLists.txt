cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(CPPServer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_subdirectory(source)

message(STATUS "Startup") 
 
# Find all *.cpp files and store in list tests
file(GLOB tests RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(testfile IN LISTS tests)
    # Get file name without directory
    get_filename_component(testname ${testfile} NAME_WE)
    add_executable(${testname} ${testfile})
    target_link_libraries(${testname} Tool)
endforeach()