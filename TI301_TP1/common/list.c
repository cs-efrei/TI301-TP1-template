//
// Created by flasque on 18/06/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"



void addHeadStd(t_std_list *p_list, int val)
{
    // val est un entier, il faut donc créer une cellule contenant cet entier et l'ajouter en tête de la liste
    return;
}

void displayStdList(t_std_list list)
{
    t_cell *curr = list.head;
    // first : display @-->
    printf("list [head @-]-->");
    if (curr == NULL)
    {
        printf("NULL");
    }
    else
    {
        while (curr != NULL)
        {
            displayCell(*curr);
            curr = curr->next;
        }
    }

    printf("\n");

    return;
}
