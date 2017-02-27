cmake_minimum_required(VERSION 3.6)

project(calcul_maya)

add_executable(calcul_maya main.cpp)

add_custom_command(TARGET calcul_maya PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/moyen/finis/calcul_maya $<TARGET_FILE_DIR:calcul_maya>)