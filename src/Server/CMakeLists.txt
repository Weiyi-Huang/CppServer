message(STATUS "Enter Server folder")
set(MODULE_SERVER Server)

file(GLOB SRC_DIR_LIST *.cpp)
# add the executable
add_executable(${MODULE_SERVER} ${SRC_DIR_LIST})

target_link_libraries(${MODULE_SERVER} Tool)