cmake_minimum_required(VERSION 3.6)

project(kolakoski_sequence)

add_executable(kolakoski_sequence main.cpp)

add_custom_command(TARGET kolakoski_sequence PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/communaute/kolakoski_sequence $<TARGET_FILE_DIR:kolakoski_sequence>)