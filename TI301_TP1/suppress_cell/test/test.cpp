// Classe utilitaire pour capturer la sortie standard (stdout / printf / cout)
//#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../../../test-framework/capture_stdout.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

// Fonctions POSIX de base compatibles Windows/MinGW
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define close _close
#define getpid _getpid
#else
#include <unistd.h>
#endif

using namespace std;

// On importe les structures et fonctions de votre TP
// (Assurez-vous qu'elles sont déclarées dans vos headers communs, ex: list.h ou cell.h)
extern "C" {
#include "../../common/list.h"
#include "../../common/getsamples.h"
// #include "cell.h"
}





// --- VOTRE TEST CASE ---
TEST_CASE("delete cell from empty list") {

// 1. On prépare la capture de l'affichage (printf)
    StdoutRedirector redirector;

// 2. On appelle DIRECTEMENT la fonction de l'étudiant à tester
// Supposons que l'étudiant doive initialiser et afficher une liste vide :
    BEGIN_TEST_CODE
    t_std_list list = EmptyStdList();
    displayStdList(list); // affichage de la liste vide pour le test
    printf("deleting value 15\n");
    removeFromStd(&list, 15);
    displayStdList(list); // affichage de la liste après tentative de suppression
    END_TEST_CODE

// 3. Récupération de ce qui a été écrit sur le terminal
    std::string output = redirector.getContent();

// 4. Nettoyage des retours à la ligne superflus pour le comparatif
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') { // Gestion du retour chariot Windows
        output.pop_back();
    }



// 5. Assertion Catch2 directe (équivalent de votre diff -EZBwb)
    REQUIRE(output == "list [head @-]-->NULL\ndeleting value 15\nlist [head @-]-->NULL");
}

TEST_CASE("delete cells from a non empty list") {

// 1. On prépare la capture de l'affichage (printf)
    StdoutRedirector redirector;

// 2. On appelle DIRECTEMENT la fonction de l'étudiant à tester
// Supposons que l'étudiant doive initialiser et afficher une liste vide :
    BEGIN_TEST_CODE
    t_std_list list = SampleStdList_1();
    displayStdList(list); // affichage de la liste vide pour le test

    printf("deleting value 2\n");
    removeFromStd(&list, 2);
    displayStdList(list); // affichage de la liste après tentative de suppression

    printf("deleting value 33\n");
    removeFromStd(&list, 33);
    displayStdList(list); // affichage de la liste après tentative de suppression

    printf("deleting value 98\n");
    removeFromStd(&list, 98);
    displayStdList(list); // affichage de la liste après tentative de suppression

    printf("deleting value 12\n");
    removeFromStd(&list, 12);
    displayStdList(list); // affichage de la liste après tentative de suppression

    END_TEST_CODE

// 3. Récupération de ce qui a été écrit sur le terminal
    std::string output = redirector.getContent();

// 4. Nettoyage des retours à la ligne superflus pour le comparatif
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') { // Gestion du retour chariot Windows
        output.pop_back();
    }



// 5. Assertion Catch2 directe (équivalent de votre diff -EZBwb)
    REQUIRE(output == "list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL\ndeleting value 2\nlist [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->NULL\ndeleting value 33\nlist [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->NULL\ndeleting value 98\nlist [head @-]-->[ 12 | @-]-->[ -7 | @-]-->NULL\ndeleting value 12\nlist [head @-]-->[ -7 | @-]-->NULL");
}
