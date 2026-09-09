#include <stdio.h>
#include "../../common/list.h"
#include "../../common/getsamples.h"
#include "countval.h"

int main()
{
    int val;
    printf("compter valeurs\n");
    printf("Saisir un entier :\n");
    scanf("%d", &val);

    // create 3 sample lists
    t_std_list list1 = SampleStdList_1();
    t_std_list list2 = RepValsStdList();
    t_std_list list3 = SortedStdList_1();

    // count the occurrences of val in each list
    int count1 = countVal(list1, val);
    printf("in list1, the value %d occurs %d times\n", val, count1);
    displayStdList(list1);

    int count2 = countVal(list2, val);
    printf("in list2, the value %d occurs %d times\n", val, count2);
    displayStdList(list2);

    int count3 = countVal(list3, val);
    printf("in list3, the value %d occurs %d times\n", val, count3);
    displayStdList(list3);

}