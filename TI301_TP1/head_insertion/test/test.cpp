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
TEST_CASE("chainage en tete - liste vide") {

// 1. On prépare la capture de l'affichage (printf)
    StdoutRedirector redirector;

// 2. On appelle DIRECTEMENT la fonction de l'étudiant à tester
// Supposons que l'étudiant doive initialiser et afficher une liste vide :
    BEGIN_TEST_CODE
    // create an empty list and display it : use EmptyStdList() and displayStdList()
    // add a cell with value 42 to the head of the list : use addHeadStd()
    // display the list again to see the new cell
    END_TEST_CODE

    t_std_list list = EmptyStdList();
    displayStdList(list); // step 1 : display empty list
    addHeadStd(&list, 42); // should add 42 to the head of the list
    displayStdList(list); // step 2 : display list after adding 42
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
    REQUIRE(output == "list [head @-]-->NULL\nlist [head @-]-->[ 42 | @-]-->NULL");
}

TEST_CASE("chainage en tete", "liste exemple SampleStdList_1") {

// 1. On prépare la capture de l'affichage (printf)
    StdoutRedirector redirector;

    BEGIN_TEST_CODE
    t_std_list list = SampleStdList_1();
    displayStdList(list); // Cette fonction fait un printf(...) à l'intérieur
    addHeadStd(&list, 42); // Ajout d'un élément pour tester l'ajout en tête
    displayStdList(list); // Affichage après ajout
    addHeadStd(&list, 66); // Ajout d'un élément pour tester l'ajout en tête
    displayStdList(list); // Affichage après ajout
    addHeadStd(&list, -99); // Ajout d'un élément pour tester l'ajout en tête
    displayStdList(list); // Affichage après ajout
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

    REQUIRE(output == "list [head @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL\nlist [head @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL\nlist [head @-]-->[ 66 | @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL\nlist [head @-]-->[ -99 | @-]-->[ 66 | @-]-->[ 42 | @-]-->[ 12 | @-]-->[ 98 | @-]-->[ -7 | @-]-->[ 2 | @-]-->NULL");
}