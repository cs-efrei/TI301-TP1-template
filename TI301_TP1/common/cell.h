//
// Created by flasque on 16/06/2022.
//

#ifndef UNTITLED_CELL_H
#define UNTITLED_CELL_H

struct s_cell
{
    int value;
    struct s_cell *next;
};

typedef struct s_cell t_cell;

/***
 * @brief display a t_cell struct
 * @param t_cell : cell to display
 * @return : void
 */
void displayCell(t_cell);

/***
 * @brief create a cell, and return a pointer to
 * @param int value
 * @return pointer to cell, whit a value, and next is NULL
 */
t_cell *createCell(int);

#endif //UNTITLED_CELL_H
