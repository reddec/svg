/* 
 * File:   svg.h
 * Author: reddec
 *
 * Created on 4 Июнь 2014 г., 18:49
 */

#ifndef SVG_H
#define	SVG_H
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#ifdef	__cplusplus
extern "C" {
#endif
#ifndef bool
#define bool unsigned char
#define true 1
#define false 0
#endif
    void svg_draw_to_file_xy(FILE *f,
            const char *color,
            double *y,
            double *x,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimail,
            bool html);
    void svg_draw_to_file_y(FILE *f,
            const char *color,
            double *y,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimail,
            bool html);


#ifdef	__cplusplus
}
#endif

#endif	/* SVG_H */

