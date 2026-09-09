
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
#include "../src/countval.h"
}





// --- VOTRE TEST CASE ---
TEST_CASE("count values - empty list") {
    StdoutRedirector redirector;
    BEGIN_TEST_CODE
    // counting values in an empty list should return 0
    t_std_list list = EmptyStdList();
    int count_empty = countVal(list, 1);
    printf("count of 1 in empty list: %d\n", count_empty);
    END_TEST_CODE


    std::string output = redirector.getContent();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') {
        output.pop_back();
    }

    REQUIRE(output == "count of 1 in empty list: 0");
}

TEST_CASE("count values - RepValsStdList (count 1)") {
    StdoutRedirector redirector;

    BEGIN_TEST_CODE
    // counting '1's in RepValsStdList() should return 4
    t_std_list list = RepValsStdList();
    int count_1 = countVal(list, 1);
    printf("count of 1 in RepValsStdList: %d\n", count_1);
    END_TEST_CODE

    std::string output = redirector.getContent();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') {
        output.pop_back();
    }

    REQUIRE(output == "count of 1 in RepValsStdList: 4");
}

TEST_CASE("count values - RepValsStdList (count 0)") {
    StdoutRedirector redirector;

    BEGIN_TEST_CODE
    // counting '0's in RepValsStdList() should return
    t_std_list list = RepValsStdList();
    int count_0 = countVal(list, 0);
    printf("count of 0 in RepValsStdList: %d\n", count_0);
    END_TEST_CODE

    std::string output = redirector.getContent();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') {
        output.pop_back();
    }

    REQUIRE(output == "count of 0 in RepValsStdList: 1");
}

TEST_CASE("count values - SortedStdList_1 (count -11)") {
    StdoutRedirector redirector;

    BEGIN_TEST_CODE
    // counting '-11's in SortedStdList_1() should return 1
    t_std_list list = SortedStdList_1();
    int count_neg11 = countVal(list, -11);
    printf("count of -11 in SortedStdList_1: %d\n", count_neg11);
    END_TEST_CODE

    std::string output = redirector.getContent();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') {
        output.pop_back();
    }

    REQUIRE(output == "count of -11 in SortedStdList_1: 1");
}

TEST_CASE("count values - SortedStdList_1 (count 42)") {
    StdoutRedirector redirector;

    BEGIN_TEST_CODE
    // counting '42's in SortedStdList_1() should return 0
    t_std_list list = SortedStdList_1();
    int count_42 = countVal(list, 42);
    printf("count of 42 in SortedStdList_1: %d\n", count_42);
    END_TEST_CODE

    std::string output = redirector.getContent();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    if (!output.empty() && output.back() == '\r') {
        output.pop_back();
    }

    REQUIRE(output == "count of 42 in SortedStdList_1: 0");
}