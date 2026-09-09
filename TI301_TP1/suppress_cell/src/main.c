#include <stdio.h>
#include "../../common/list.h"
#include "../../common/getsamples.h"
#include "../../common/svg.h"

int main()
{
    // define a variable of type t_std_list
    t_std_list votre_variable_liste;
    // initialize it with a non empty list
    votre_variable_liste = SampleStdList_1();
    // visualize the list with the displayStdList function
    displayStdList(votre_variable_liste);
    listToSvg(votre_variable_liste ,"list_before.svg");
     // try deleting cells
    // visualize the list with the displayStdList function

    // pour avoir une visualisation, ficher svg dans le dossier output
    listToSvg(votre_variable_liste ,"list_after.svg");

    return 0;
}