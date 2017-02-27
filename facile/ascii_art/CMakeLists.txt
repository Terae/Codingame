cmake_minimum_required(VERSION 3.6)

project(ascii_art)

add_executable(ascii_art main.cpp)

add_custom_command(TARGET ascii_art PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/facile/ascii_art $<TARGET_FILE_DIR:ascii_art>)