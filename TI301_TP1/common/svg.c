//
// Created by flasque on 15/07/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define OUTPUT_FILE_PATH "../../../TP1/chainage_en_tete/output/"


void listToSvg(t_std_list list, char *filename)
{
    char full_filename[256];
    char output_dir[256];
    #ifdef SVG_OUTPUT_DIR
        snprintf(output_dir, sizeof(output_dir), "%s/", SVG_OUTPUT_DIR);
    #else
        snprintf(output_dir, sizeof(output_dir), "%s", OUTPUT_FILE_PATH);
    #endif
    // Create the full path for the output file
    // step 1: use OUTPUT_FILE_PATH as the base path
    snprintf(full_filename, sizeof(full_filename), "%s", output_dir);
    // step 2: append the filename to the base path, use strcat wtih the full_filename as the destination and filename as the source
    strcat(full_filename, filename);

    FILE *file = fopen(full_filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // SVG header
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"200\">\n");

    int x = 50; // Starting x-coordinate
    int y = 50; // y-coordinate for all nodes
    int nodeWidth = 80;
    int nodeHeight = 40;
    int arrowLength = 40;

    t_cell * current = list.head;
    // draw a rectangle for the list variable
    fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"none\" stroke=\"black\" />\n", x - 20, y, nodeWidth + 40, nodeHeight);
    // draw the text 'head' : centered in the left half of the rectangle
    fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"16\" fill=\"black\" text-anchor=\"middle\" alignment-baseline=\"middle\">head</text>\n",
            x + (nodeWidth + 40) / 4, y + nodeHeight / 2);
    // drwaw the text '@' : centered in the right half of the rectangle
    fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"16\" fill=\"black\" text-anchor=\"middle\" alignment-baseline=\"middle\">@</text>\n",
            x + nodeWidth-5, y +20);
    // add a square containing '@' on the right part of the rectangle
    fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"none\" stroke=\"black\" />\n", x + (nodeWidth + 40) / 2, y+5 , 30, nodeHeight -10);
    //update left offset for subsequent nodes
    x += nodeWidth + arrowLength;
    // draw an arrow from the right part of the rectangle to the first node in any case

    int arrowStartX = x - arrowLength;
    int arrowStartY = y + nodeHeight / 2;
    int arrowEndX = x;
    int arrowEndY = y + nodeHeight / 2;

    fprintf(file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" marker-end=\"url(#arrowhead)\" />\n",
            arrowStartX, arrowStartY, arrowEndX, arrowEndY);


    while (current != NULL) {
        // Draw the rectangle for the node
        fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"lightblue\" stroke=\"black\" />\n", x, y, nodeWidth, nodeHeight);

        // Draw the value inside the rectangle, on the left part
        fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"16\" fill=\"black\" text-anchor=\"middle\" alignment-baseline=\"middle\">%d</text>\n",
                x + nodeWidth / 4, y + nodeHeight / 2, current->value);
        // draw a square containing '@' on the right part of the rectangle
        fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"white\" stroke=\"black\" />\n", x + nodeWidth / 2, y, nodeWidth / 2, nodeHeight);
        fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"16\" fill=\"black\" text-anchor=\"middle\" alignment-baseline=\"middle\">@</text>\n",
                x + 3 * nodeWidth / 4, y + nodeHeight / 2);
        // Draw the arrow to the next node (if not the last node)

            int arrowStartX = x + nodeWidth-10;
            int arrowStartY = y + nodeHeight / 2;
            int arrowEndX = x + nodeWidth + arrowLength;
            int arrowEndY = y + nodeHeight / 2;

            fprintf(file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" marker-end=\"url(#arrowhead)\" />\n",
                    arrowStartX, arrowStartY, arrowEndX, arrowEndY);


        // Move to the next node
        x += nodeWidth + arrowLength;
        current = current->next;
    }
    // add NULL text at the end of the list
    fprintf(file, "<text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"16\" fill=\"black\" text-anchor=\"middle\" alignment-baseline=\"middle\">NULL</text>\n",
            x + 20, y + nodeHeight / 2);

    // Define the arrowhead marker
    fprintf(file, "<defs>\n");
    fprintf(file, "<marker id=\"arrowhead\" markerWidth=\"10\" markerHeight=\"7\" refX=\"10\" refY=\"3.5\" orient=\"auto\">\n");
    fprintf(file, "<polygon points=\"0 0, 10 3.5, 0 7\" fill=\"black\" />\n");
    fprintf(file, "</marker>\n");
    fprintf(file, "</defs>\n");

    // Close the SVG
    fprintf(file, "</svg>\n");

    fclose(file);
}