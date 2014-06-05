svg
===

Lightweight C plotting library without special dependencies for Linux and Win

## API

Full version

```c++
void svg_draw_to_file_xy(
            FILE *f,
            const char *color,
            double *y,
            double *x,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimal,
            bool html)
```
            
Short version with auto generated X (from 0 to count-1)

```c++
void svg_draw_to_file_y(
            FILE *f,
            const char *color,
            double *y,
            size_t count,
            const char *xlabel,
            const char *ylabel,
            int decimal,
            bool html)
```

* **f** - reference to opened file for writing result
* **color** - line color
* **y** - array with values
* **x** - sorted array with x values from minimum to maximum
* **count** - elements count (not bytes)
* **decimal** - number after dot in float
* **html** - generate or not html page with inline svg. Usefull for Windows users, which do not have other good SVG viewer then browsers
