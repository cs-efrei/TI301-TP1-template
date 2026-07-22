//
// Created by flasque on 03/09/2022.
//

#include <stdlib.h>
#include "getsamples.h"

// fonctions locales

void headChain(t_std_list *p_list, int val)
{
    t_cell * p_new = createCell(val);
    p_new->next = p_list->head;
    p_list->head = p_new;

    return;
}

void headChainHt(t_ht_list *p_list, int val)
{
    t_cell *p_nouv;
    p_nouv = createCell(val);

    if (p_list->head == NULL)
    {
        p_list->tail = p_nouv;
    }

    p_nouv->next = p_list->head;
    p_list->head = p_nouv;

    return;

}

t_ht_list createHtListFromArray(int *vals, int nb)
{
    t_ht_list sample = EmptyHtList();
    for (int cpt = 0; cpt < nb; cpt++)
    {
        headChainHt(&sample, vals[cpt]);
    }

    return sample;
}

t_std_list createListFromArray(int *vals, int nb)
{
    t_std_list sample;
    sample.head = NULL;
    for (int cpt = 0; cpt < nb; cpt++)
    {
        headChain(&sample, vals[cpt]);
    }

    return sample;
}

// fonctions exportées

t_std_list EmptyStdList()
{
    t_std_list sample;
    sample.head = NULL;
    return sample;
}

t_std_list OneCellStdList()
{
    t_std_list sample;
    t_cell * p_new = createCell(19);

    sample.head = p_new;

    return sample;
}

t_std_list SampleStdList_1()
{
    t_std_list sample;
    int vals[4] = {2, -7, 98, 12};

    sample = createListFromArray(vals, 4);

    return sample;
}


t_std_list SampleStdList_2()
{
    t_std_list sample;
    int vals[7] = {15, 0, -3, 6, 89, -17, 42};

    sample = createListFromArray(vals, 7);
    return sample;
}


t_std_list SortedStdList_1()
{
    t_std_list sample;
    int vals[7] = {45, 12, 6, 0, -1, -4, -11};


    sample = createListFromArray(vals, 7);

    return sample;
}


t_std_list SortedStdList_2()
{
    t_std_list sample;
    int vals[10] = {130, 2, 1, 0, -8, -9, -24, -47, -49, -50};

    sample = createListFromArray(vals, 10);
    return sample;
}

t_std_list RandomSortedStdList()
{
    t_std_list sample;
    int vals[12];

    vals[0] = rand() % 50 + 30;

    for (int cpt = 1; cpt < 12; cpt++)
    {
        int offset = rand() % 6 + 1;
        vals[cpt] = vals[cpt - 1] - offset;
    }

    sample = createListFromArray(vals, 12);
    return sample;
}


t_std_list RandomLongStdList()
{
    t_std_list sample;
    int vals[30];
    for (int cpt = 0; cpt < 30; cpt++)
    {
        vals[cpt] = rand() % 100 - 50;
    }

    sample = createListFromArray(vals, 30);

    return sample;
}

t_std_list RepValsStdList()
{
    t_std_list sample;
    int vals[10] = {1, 2, 1, 1, 3, 2, 3, 1, 0, 2};

    sample = createListFromArray(vals, 10);
    return sample;
}

t_ht_list EmptyHtList()
{
    t_ht_list sample;

    sample.head = sample.tail = NULL;

    return sample;
}


t_ht_list OneCellHtList()
{
    t_ht_list sample = EmptyHtList();
    headChainHt(&sample, 19);

    return sample;
}

t_ht_list SampleHtList_1()
{
    t_ht_list sample;
    int vals[4] = {2, -7, 98, 12};

    sample = createHtListFromArray(vals, 4);

    return sample;
}


t_ht_list SampleHtList_2()
{
    t_ht_list sample;
    int vals[7] = {15, 0, -3, 6, 89, -17, 42};

    sample = createHtListFromArray(vals, 7);
    return sample;
}


t_ht_list SortedHtList_1()
{
    t_ht_list sample;
    int vals[7] = {45, 12, 6, 0, -1, -4, -11};


    sample = createHtListFromArray(vals, 7);

    return sample;
}


t_ht_list SortedHtList_2()
{
    t_ht_list sample;
    int vals[10] = {130, 2, 1, 0, -8, -9, -24, -47, -49, -50};

    sample = createHtListFromArray(vals, 10);
    return sample;
}

t_ht_list RandomSortedHtList()
{
    t_ht_list sample;
    int vals[12];

    vals[0] = rand() % 50 + 30;

    for (int cpt = 1; cpt < 12; cpt++)
    {
        int offset = rand() % 6 + 1;
        vals[cpt] = vals[cpt - 1] - offset;
    }

    sample = createHtListFromArray(vals, 12);
    return sample;
}


t_ht_list RandomLongHtList()
{
    t_ht_list sample;
    int vals[30];
    for (int cpt = 0; cpt < 30; cpt++)
    {
        vals[cpt] = rand() % 100 - 50;
    }

    sample = createHtListFromArray(vals, 30);

    return sample;
}

t_ht_list RepValsHtList()
{
    t_ht_list sample;
    int vals[10] = {1, 2, 1, 1, 3, 2, 3, 1, 0, 2};

    sample = createHtListFromArray(vals, 10);
    return sample;
}