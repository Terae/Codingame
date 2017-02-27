cmake_minimum_required(VERSION 3.6)

project(temperatures)

add_executable(temperatures main.cpp)

add_custom_command(TARGET temperatures PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/facile/temperatures $<TARGET_FILE_DIR:temperatures>)