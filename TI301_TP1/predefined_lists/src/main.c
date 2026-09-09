#include <stdio.h>
#include "../../common/list.h"
#include "../../common/getsamples.h"
#include "../../common/svg.h"

int main()
{
    // define a variable of type t_std_list

    // initialize it with an empty list

     // visualize the list with the displayStdList function

    // create a svg file in the output directory to have a pretty diagram
    listToSvg(votre_variable_liste ,"empty_list.svg");

    // initialize the list with a sample list (SampleStdList_1)

    // visualize the list with the displayStdList function

    // create a svg file in the output directory to have a pretty diagram
    listToSvg(votre_variable_liste ,"sample_list_1.svg");
    return 0;
}
