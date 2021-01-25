/* 
 * File:   svg.h
 * Author: reddec
 *
 * Created on 4 June 2014, 18:49
 */

#ifndef SVG_H
#define	SVG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif
#ifndef bool
#define bool unsigned char
#define true 1
#define false 0
#endif
    /**
     * Plot x-y data to SVG file
     * @param f opened file for writing
     * @param color line color
     * @param y data values
     * @param x ordered x values from min to max
     * @param count elements count
     * @param xlabel label for Ax
     * @param ylabel label for Ay
     * @param decimal count of numbers after dot in float values
     * @param html wrap SVG content to HTML or not
     */
    void svg_draw_to_file_xy(FILE *f,
            const char *color,
            double *y,
            double *x,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimal,
            bool html);

    /**
     * Plot y data to SVG file. X auto generates from 0 to count-1
     * @param f opened file for writing
     * @param color line color
     * @param y data values
     * @param count elements count
     * @param xlabel label for Ax
     * @param ylabel label for Ay
     * @param decimal count of numbers after dot in float values
     * @param html wrap SVG content to HTML or not
     */
    void svg_draw_to_file_y(FILE *f,
            const char *color,
            double *y,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimal,
            bool html);


#ifdef	__cplusplus
}
#endif

#endif	/* SVG_H */

