cmake_minimum_required(VERSION 3.6)

project(bender_part1)

add_executable(bender_part1 main.cpp)

add_custom_command(TARGET bender_part1 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/moyen/en_cours/bender_part1 $<TARGET_FILE_DIR:bender_part1>)