cmake_minimum_required(VERSION 3.10)
project(CPPServer)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
 
include_directories("${PROJECT_BINARY_DIR}")
 
# Find all cpp files and store in list codes
file(GLOB codes RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
foreach(code IN LISTS codes)
    # Get file name without directory
    get_filename_component(code_name ${code} NAME_WE)
    add_executable(${code_name} ${code})
endforeach()