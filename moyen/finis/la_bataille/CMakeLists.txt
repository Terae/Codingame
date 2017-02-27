cmake_minimum_required(VERSION 3.6)

project(bender_part1)

add_executable(la_bataille main.cpp)

add_custom_command(TARGET la_bataille PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/moyen/finis/la_bataille $<TARGET_FILE_DIR:la_bataille>)