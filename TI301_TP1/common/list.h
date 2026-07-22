//
// Created by flasque on 18/06/2022.
//

#ifndef UNTITLED_LIST_H
#define UNTITLED_LIST_H

#include "cell.h"


typedef struct s_std_list
{
    t_cell *head;
} t_std_list;

typedef struct s_ht_list
{
    t_cell *head;
    t_cell *tail;
} t_ht_list;


void displayStdList(t_std_list);

void addHeadStd(t_std_list *, int);

#endif //UNTITLED_LIST_H
