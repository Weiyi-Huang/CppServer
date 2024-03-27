message(STATUS "Enter Tool folder")
set(MODULE_TOOL Tool)

aux_source_directory(. TOOL_SOURCE)
add_library(${MODULE_TOOL} ${TOOL_SOURCE})

target_include_directories(${MODULE_TOOL} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})