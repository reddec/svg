#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "svg.h"

int main(int argc, char** argv)
{
    int size = 410, i;
    double *data = (double*) malloc(sizeof (double)*size);/*Allocate test y array*/
    double *x = (double*) malloc(sizeof (double)*size);   /*Allocate test x array*/
    FILE *f;
    for (i = 0; i < size; ++i)                             /*Fill x and y values*/
    {
        data[i] = 50 + 50 * sin(i * 0.03);
        x[i] = i;
    }
    f = fopen("sin.svg", "w");                             /*Open result file*/
    /* Plot graphic with red (#ff0000) lines as svg image only */
    svg_draw_to_file_xy(f, "#ff0000", data, x, size, "x", "sin(x)", 2, false); 
    fclose(f);                                             /*Close result file*/
    free(data);                                            /* Free y array */
    free(x);                                               /* Free x array */
    return (EXIT_SUCCESS);
}
