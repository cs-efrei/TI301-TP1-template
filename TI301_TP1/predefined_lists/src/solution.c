#include <stdio.h>
#include "../../common/list.h"
#include "../../common/getsamples.h"

int main()
{
    // définissez une variable de type t_std_list
    t_std_list mylist;

    // initialisez la avec une liste vide
    mylist = EmptyStdList();

    // visualisez la avec la fonction fournie
    displayStdList(mylist);

    return 0;
}
