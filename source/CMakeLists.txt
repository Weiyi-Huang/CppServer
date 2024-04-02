message(STATUS "Enter source folder")

set(ModuleName Tool)
aux_source_directory(. SourceCode)
add_library(${ModuleName} ${SourceCode})

target_include_directories(${ModuleName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})