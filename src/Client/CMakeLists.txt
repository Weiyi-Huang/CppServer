message(STATUS "Enter Client folder")
set(MODULE_Client Client)
 
# add the executable
add_executable(${MODULE_Client} client.cpp)

target_link_libraries(${MODULE_Client} Tool)