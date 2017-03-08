cmake_minimum_required(VERSION 3.6)

project(codingame)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

## Quelques définitions pour compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

## Mise à jour des submodules Git.
execute_process(
        COMMAND git submodule update --init
        WORKING_DIRECTORY ${codingame_SOURCE_DIR}
)

## Pour les algos utilisant un réseau Petri
add_subdirectory(petri)

function(compile_petri TARGET_NAME PATH_TO_DOC OUTPUT)
    add_custom_target(${TARGET_NAME}
            COMMAND ${root_SOURCE_DIR}/petri/petri --update --arch ${BITS} ${CMAKE_CURRENT_SOURCE_DIR}/${PATH_TO_DOC}
            )
    add_dependencies(${TARGET_NAME} PetriEditor)
endfunction()

# Permet d'avoir le même code en local et sur codingame.com (avec un '#ifdef _CLION_
#                                                                           lire_fichiers_tests
#                                                                     #endif')
add_definitions(-D_CLION_)

####################
### ENTRAINEMENT ###
####################
add_subdirectory(facile/ascii_art)
add_subdirectory(facile/chevaux_de_course)
add_subdirectory(facile/chuck_norris)
add_subdirectory(facile/defibrillateurs)
add_subdirectory(facile/la_descente)
add_subdirectory(facile/mars_lander)
add_subdirectory(facile/mime_type)
add_subdirectory(facile/onboarding)
add_subdirectory(facile/power_of_Thor_part1)
add_subdirectory(facile/temperatures)

add_subdirectory(moyen/finis/bender_part1)
add_subdirectory(moyen/finis/calcul_maya)
add_subdirectory(moyen/finis/des_nains_sur_des_epaules_de_geants)
add_subdirectory(moyen/finis/dont_panic_part1)
add_subdirectory(moyen/finis/la_bataille)
add_subdirectory(moyen/finis/le_cadeau)
add_subdirectory(moyen/finis/pertes_en_bourse)
add_subdirectory(moyen/finis/shadows_of_the_knight_part1)
add_subdirectory(moyen/finis/skynet_revolution_part1)
add_subdirectory(moyen/finis/suite_de_Conway)
add_subdirectory(moyen/finis/teads_sponsored_challenge)
add_subdirectory(moyen/finis/there_is_no_spoon_part1)

add_subdirectory(difficile/en_cours/power_of_Thor_part2)
add_subdirectory(difficile/en_cours/skynet_revolution_part2)
add_subdirectory(difficile/finis/montagnes_russes)

add_subdirectory(tres_difficile/finis/shadows_of_the_knight_part2)

###################
### COMPETITION ###
###################
add_subdirectory(taille_de_code/dont_panic)
add_subdirectory(taille_de_code/power_of_Thor)
add_subdirectory(taille_de_code/temperatures)

add_subdirectory(challenge/ghost_in_the_cell)

####################
### CONTRIBUTION ###
####################
add_subdirectory(communaute/expressions_parenthesees)
add_subdirectory(communaute/kolakoski_sequence)
add_subdirectory(communaute/la_gravite)
add_subdirectory(communaute/le_plus_rapide)
add_subdirectory(communaute/magic_stones)