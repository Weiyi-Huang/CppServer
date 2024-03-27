message(STATUS "Enter Server folder")
set(MODULE_SERVER Server)

# add the executable
add_executable(${MODULE_SERVER} server.cpp)

target_link_libraries(${MODULE_SERVER} Tool)