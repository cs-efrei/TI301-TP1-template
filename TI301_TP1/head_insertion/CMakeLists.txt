cmake_minimum_required(VERSION 3.21)

# Étape 1 : On force l'activation du C et du C++ (CXX) pour que <iostream> soit reconnu
project(head_insertion C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14) # Standard C++ requis par Catch2 et iostream

# =========================================================================
# NOUVEAU : CONFIGURATION DE LA LIBRAIRIE PARTAGÉE (SHARED)
# =========================================================================

# 1. On force la sortie des exécutables ET des librairies dynamiques (.dll/.so)
# dans le même répertoire "bin" de votre dossier cmake-build-debug.
# Cela garantit que Windows trouvera la .dll au moment de lancer le test.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 2. On crée la librairie partagée compilée une seule fois
if(NOT TARGET catch_runner)
    add_library(catch_runner SHARED ../../test-framework/catch_precomp.cpp)

    # Propriétés spécifiques à Windows pour la DLL
    if(WIN32)
        set_target_properties(catch_runner PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    endif()
endif()
# NOUVEAU : On transmet le chemin absolu directement à la .dll
target_compile_definitions(catch_runner PRIVATE
        CLARITY_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/output"
)
# 3. Indispensable pour Windows : force l'exportation des symboles dans la .dll
# (Sous Linux/Mac, cela n'a pas d'effet négatif)
set_target_properties(catch_runner PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# =========================================================================

# Déclaration des fichiers sources
set(STUDENT_MAIN src/main.c)
set(COMMON_SRC
        ../common/cell.c
        ../common/list.c
        ../common/getsamples.c
        ../common/svg.c
        ../common/svg.h
)

# Cible exécutable standard pour l'étudiant
set(SRC ${STUDENT_MAIN} ${COMMON_SRC})
add_executable(${PROJECT_NAME}-run ${SRC})

# -------------------------------------------------------------------------
# CONFIGURATION EXPLICITE DE LA CIBLE DE TEST
# -------------------------------------------------------------------------
set(TEST
        test/test.cpp
)

set(TEST_HELPERS
        ../../test-framework/test_helpers.cpp
        # ATTENTION : catch_precomp.cpp a été retiré d'ici !
)

enable_testing()  # Active le support des tests dans CMake

add_executable(${PROJECT_NAME}-test ${TEST_HELPERS} ${TEST} ${COMMON_SRC})

# =========================================================================
# NOUVEAU : ÉDITION DES LIENS
# =========================================================================
# On lie la librairie partagée contenant Catch2 à l'exécutable de test
target_link_libraries(${PROJECT_NAME}-test PRIVATE catch_runner)
# =========================================================================

add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)

# On indique à l'IDE et à l'étudiant que cette cible est un test JetBrains Edu
set_target_properties(${PROJECT_NAME}-test PROPERTIES
        LINKER_LANGUAGE CXX # Force l'utilisation du compilateur C++ (g++)
)

# Configuration des répertoires d'inclusion
target_include_directories(${PROJECT_NAME}-test PRIVATE
        test
        ../common
        ${CMAKE_CURRENT_SOURCE_DIR}/../test-framework
)

# On s'assure que l'exécutable de l'étudiant est compilé
add_dependencies(${PROJECT_NAME}-test ${PROJECT_NAME}-run)

# Passage des constantes de préprocesseur au fichier test.cpp
target_compile_definitions(${PROJECT_NAME}-test PRIVATE
        STUDENT_EXE="$<TARGET_FILE:${PROJECT_NAME}-run>"
        STUDENT_SRC="${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
        CLARITY_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/output"
)

target_compile_definitions(${PROJECT_NAME}-run PRIVATE
        SVG_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/output")