#include "svg.h"
#define MARKER_ARROW fprintf(f,"<marker id=\"markerArrow\" markerWidth=\"13\" markerHeight=\"13\" refX=\"7\" refY=\"6\" orient=\"auto\">\n<path d=\"M2,8 L11,6 L2,4 Z\" style=\"fill: #000000;stroke: black;\" />    </marker>")

#define cordx(var) (((var)-ctx->ofx)*(ctx->mx))
#define cordy(var) (((var)-ctx->ofy)*(ctx->my))
//TODO: Refactoring

typedef struct svg_context
{
    char stroke_color[20];
    char fill_color[20];
    char text_family[30];
    char dash_array[30];
    unsigned int text_size;
    unsigned int stroke_width;

    bool stroke, fill;
    /*document*/
    int padding;
    int width, height;
    bool flip;
    int decimals;
    float mx, my, ofx, ofy;
    FILE *f;

} svg_context;

struct svg_context * svg_create(FILE *f)
{
    struct svg_context *ctx = (struct svg_context*) malloc(sizeof (svg_context));
    strcpy(ctx->fill_color, "#ffffff\0");
    strcpy(ctx->stroke_color, "#000000\0");
    strcpy(ctx->text_family, "monospace\0");
    strcpy(ctx->dash_array, "\0");
    ctx->text_size = 14;
    ctx->stroke_width = 1;
    ctx->stroke = true;
    ctx->fill = false;
    ctx->f = f;
    ctx->width = 1920;
    ctx->height = 1080;
    ctx->padding = 50;
    ctx->flip = true;
    ctx->mx = 1;
    ctx->my = 1;
    ctx->ofx = 0;
    ctx->ofy = 0;
    ctx->decimals = 2;
    return ctx;
}

void svg_finish(svg_context *ctx)
{
    fprintf(ctx->f, "</svg>");
    free(ctx);
}

void svg_doc(svg_context *ctx)
{
	fprintf(ctx->f, "<svg version=\"1.2\" width=\"%i\" height=\"%i\" xmlns=\"http://www.w3.org/2000/svg\">", ctx->width, ctx->height);
}

void svg_tag_start(svg_context *ctx, const char *name, bool use_opt)
{
    fprintf(ctx->f, "\n<%s %s", name, (use_opt ? " " : ">"));
}

void svg_opt_end(svg_context *ctx, bool selfclose)
{
    fprintf(ctx->f, " %s>\n", (selfclose ? "/" : ""));
}

void svg_optf(svg_context *ctx, const char *key, float v)
{
    fprintf(ctx->f, "%s=\"%f\" ", key, v);
}

void svg_opti(svg_context *ctx, const char *key, int v)
{
    fprintf(ctx->f, "%s=\"%i\" ", key, v);
}

void svg_opta(svg_context *ctx, const char *key, const char *v)
{
    fprintf(ctx->f, "%s=\"%s\" ", key, v);
}

void svg_opt_fill(svg_context *ctx)
{
    fprintf(ctx->f, "fill=\"%s\" ", ctx->fill_color);
}

void svg_opt_fill_none(svg_context *ctx)
{
    fprintf(ctx->f, "fill=\"none\" ");
}

void svg_opt_stroke(svg_context *ctx)
{
    fprintf(ctx->f, "stroke=\"%s\" stroke-width=\"%i\" stroke-dasharray=\"%s\" ", ctx->stroke_color, ctx->stroke_width, ctx->dash_array);
}

void svg_opt_long_beg(svg_context *ctx, const char *key)
{
    fprintf(ctx->f, "%s=\"", key);
}

void svg_opt_long_end(svg_context *ctx)
{
    fprintf(ctx->f, "\" ");
}

void svg_rotate(svg_context *ctx, int angle, int cx, int cy)
{
    fprintf(ctx->f, "translate(%i,%i) rotate(%i) translate(%i,%i) ",
            cx, cy, angle, -cx, -cy);
}

void svg_translate(svg_context *ctx, int dx, int dy)
{
    fprintf(ctx->f, "translate(%i,%i) ", dx, dy);
}

void svg_scale(svg_context *ctx, float sx, float sy)
{
    fprintf(ctx->f, "scale(%f,%f) ", sx, sy);
}

void svg_tag_end(svg_context *ctx, const char *name)
{
    fprintf(ctx->f, "\n</%s>", name);
}

void svg_line(svg_context *ctx, float x1, float y1, float x2, float y2)
{
    svg_tag_start(ctx, "line", true);
    {
        svg_optf(ctx, "x1", cordx(x1));
        svg_optf(ctx, "y1", cordy(y1));
        svg_optf(ctx, "x2", cordx(x2));
        svg_optf(ctx, "y2", cordy(y2));
        svg_opt_stroke(ctx);
    }
    svg_opt_end(ctx, true);
}

void svg_arrow(svg_context *ctx, float x1, float y1, float x2, float y2)
{
    svg_tag_start(ctx, "line", true);
    {
        svg_optf(ctx, "x1", cordx(x1));
        svg_optf(ctx, "y1", cordy(y1));
        svg_optf(ctx, "x2", cordx(x2));
        svg_optf(ctx, "y2", cordy(y2));
        svg_opta(ctx, "marker-end", "url(#markerArrow)");
        svg_opt_stroke(ctx);
    }
    svg_opt_end(ctx, true);
}

void svg_path_data(svg_context *ctx, double *x, double *y, size_t count)
{
    size_t i;
    svg_tag_start(ctx, "path", true);
    {
        fprintf(ctx->f, " d=\"");
        for (i = 0; i < count; ++i)
        {
            fprintf(ctx->f, "%s%f,%f\n", (i == 0 ? "M" : " L"), cordx(x[i]), cordy(y[i]));
        }
        fprintf(ctx->f, "\" ");
        svg_opt_fill_none(ctx);
        svg_opt_stroke(ctx);
    }
    svg_opt_end(ctx, true);
}

void svg_rect(svg_context *ctx, float x, float y, float w, float h)
{
    svg_tag_start(ctx, "rect", true);
    svg_optf(ctx, "x", cordx(x));
    svg_optf(ctx, "y", cordy(y));
    svg_optf(ctx, "width", w);
    svg_optf(ctx, "height", h);
    svg_opt_fill(ctx);
    svg_opt_stroke(ctx);
    svg_opt_end(ctx, true);
}

void svg_text(svg_context *ctx, const char *txt, float x, float y)
{
    svg_tag_start(ctx, "text", true);
    svg_optf(ctx, "x", cordx(x));
    svg_optf(ctx, "y", cordy(y));
    svg_opti(ctx, "font-size", ctx->text_size);
    svg_opta(ctx, "font-family", ctx->text_family);
    svg_opt_fill(ctx);
    svg_opt_stroke(ctx);
    svg_opt_end(ctx, false);
    fprintf(ctx->f, "%s", txt);
    svg_tag_end(ctx, "text");
}

void svg_set_fill(svg_context *ctx, const char *color)
{
    strcpy(ctx->fill_color, color);
}

void svg_set_dash(svg_context *ctx, const char *dash)
{
    strcpy(ctx->dash_array, dash);
}

void svg_set_stroke(svg_context *ctx, const char *color)
{
    strcpy(ctx->stroke_color, color);
}

void max_min(double *data, double *min, double *max, size_t count)
{
    size_t i;
    for (i = 0; i < count; ++i)
        if (i == 0)
        {
            *min = data[i];
            *max = data[i];
        }
        else if (data[i]<*min)
            *min = data[i];
        else if (data[i]>*max)
            *max = data[i];

}

void svg_draw_to_file_xy(FILE *f, const char *color,
                         double *y, double *x, size_t count,
                         const char *xlabel, const char *ylabel,
                         int decimal, bool html)
{
    double min_y, min_x, max_y, max_x;
    double height, width, t;
    float mx, my;
    double tx, ty;
    char temp[30];
    svg_context *ctx;
    if (count == 0)return;
    min_x = x[0];
    max_x = x[count - 1];
    /* Find minimum and maximium of Y axis. X mast be already ordered*/
    max_min(y, &min_y, &max_y, count);
    width = max_x - min_x;
    height = max_y - min_y;

    /* Begin document */
    ctx = svg_create(f);
    if (html)
    {
        fprintf(f, "<!DOCTYPE html>\n");
        fprintf(f, "<html>\n");
        fprintf(f, "<body>\n");
    }
    svg_doc(ctx);
    ctx->decimals = decimal;
    mx = (ctx->width - 2 * ctx->padding) / (float) width;
    my = (ctx->height - 2 * ctx->padding) / (float) height;
    fprintf(f, "<defs>\n");
    MARKER_ARROW;
    fprintf(f, "</defs>\n");
    svg_set_fill(ctx, "#ffffff");
    svg_rect(ctx, 0, 0, ctx->width, ctx->height);

    /*Vertical lines*/
    svg_set_stroke(ctx, "#aaaaaa");

    ctx->stroke_width = 1;
    for (t = min_x + width / 10.0; t / max_x < 0.96; t = t + width / 10)
    {
        tx = (t - min_x) * mx + ctx->padding;

        svg_set_dash(ctx, "5,5");
        svg_line(ctx, tx, ctx->height - ctx->padding, tx, ctx->padding);
        svg_set_dash(ctx, "");
        svg_tag_start(ctx, "g", true);
        {
            sprintf(temp, "%.*f", ctx->decimals, t);
            svg_opt_long_beg(ctx, "transform");
            svg_rotate(ctx, -90, tx - 3, ctx->height - ctx->padding - 10);
            svg_translate(ctx, tx - 3, ctx->height - ctx->padding - 10);
            svg_opt_long_end(ctx);
            svg_opt_end(ctx, false);

            svg_text(ctx, temp, 0, 0);
        }
        svg_tag_end(ctx, "g");
    }
    /*Horizontal lines*/
    svg_set_stroke(ctx, "#aaaaaa");

    ctx->stroke_width = 1;
    for (t = min_y + height / 10.0; t / max_y < 0.96; t = t + height / 10.0)
    {
        ty = ctx->height - (t - min_y) * my - ctx->padding;
        svg_set_dash(ctx, "5,5");
        svg_line(ctx, ctx->padding, ty, ctx->width - ctx->padding, ty);
        svg_set_dash(ctx, "");
        sprintf(temp, "%.*f", ctx->decimals, t);
        svg_text(ctx, temp, ctx->padding + 3, ty - 3);
    }

    svg_tag_start(ctx, "g", true);
    {
        ctx->mx = mx;
        ctx->my = my;
        ctx->ofx = min_x;
        ctx->ofy = min_y;
        svg_opt_long_beg(ctx, "transform");
        svg_translate(ctx, ctx->padding, ctx->padding);
        svg_scale(ctx, 1, -1);
        svg_translate(ctx, 0, -ctx->height + 2 * ctx->padding);
        svg_opt_long_end(ctx);
        svg_opt_end(ctx, false);


        /* Plot data */
        svg_set_stroke(ctx, color);
        svg_set_dash(ctx, "");
        svg_path_data(ctx, x, y, count);
        svg_set_stroke(ctx, "#000000");

    }
    svg_tag_end(ctx, "g");
    ctx->mx = 1;
    ctx->my = 1;
    ctx->ofx = 0;
    ctx->ofy = 0;
    sprintf(temp, "%.*f", ctx->decimals, max_y);
    svg_text(ctx, temp, ctx->padding + 3, ctx->padding);
    sprintf(temp, "%.*f", ctx->decimals, max_x);
    svg_tag_start(ctx, "g", true);
    {
        svg_opt_long_beg(ctx, "transform");
        svg_rotate(ctx, -90, ctx->width - ctx->padding - ctx->text_size / 2,
                   ctx->height - ctx->padding - ctx->text_size);
        svg_translate(ctx, ctx->width - ctx->padding - ctx->text_size,
                      ctx->height - ctx->padding - ctx->text_size);
        svg_opt_long_end(ctx);
        svg_opt_end(ctx, false);
        svg_text(ctx, temp, 0, 0);
    }
    svg_tag_end(ctx, "g");
    /* sprintf(temp, "%.*f", ctx->decimals, min_y);
     svg_text(ctx, temp, ctx->padding + 3, ctx->height - ctx->padding + ctx->text_size / 2);*/
    /*axis*/
    svg_set_fill(ctx, "#000000");
    svg_set_stroke(ctx, "#000000");
    svg_set_dash(ctx, "");
    ctx->stroke_width = 2;
    ctx->text_size = 0.6 * (ctx->padding);

    /* Y */
    svg_set_stroke(ctx, "#000000");
    svg_set_dash(ctx, "");

    svg_arrow(ctx,
              ctx->padding,
              ctx->height - ctx->padding / 2,
              ctx->padding,
              ctx->padding / 2);
    ctx->stroke_width = 1;
    svg_text(ctx, ylabel,
             ctx->padding + ctx->text_size / 4.0,
             ctx->padding / 2 + ctx->text_size / 4.0);
    sprintf(temp, "%.*f", ctx->decimals, x[0]);
    ctx->text_size = 14;
    svg_tag_start(ctx, "g", true);
    {
        svg_opt_long_beg(ctx, "transform");
        svg_rotate(ctx, -90, ctx->padding - ctx->text_size / 2,
                   ctx->height - ctx->padding - ctx->text_size);
        svg_translate(ctx, ctx->padding - ctx->text_size,
                      ctx->height - ctx->padding - ctx->text_size);
        svg_opt_long_end(ctx);
        svg_opt_end(ctx, false);
        svg_text(ctx, temp, 0, 0);
    }
    svg_tag_end(ctx, "g");
    ctx->text_size = 0.6 * (ctx->padding);

    ctx->stroke_width = 2;

    /* X */
    svg_arrow(ctx,
              ctx->padding / 2,
              ctx->height - ctx->padding,
              ctx->width - ctx->padding / 2,
              ctx->height - ctx->padding);
    ctx->stroke_width = 1;
    ctx->text_size = 0.6 * (ctx->padding);

    svg_text(ctx, xlabel,
             ctx->width - strlen(xlabel) * ctx->text_size,
             ctx->height - ctx->padding - ctx->text_size / 2.0);
    sprintf(temp, "%.*f", ctx->decimals, min_y);
    ctx->text_size = 14;
    svg_text(ctx, temp, ctx->padding + 3, ctx->height - ctx->padding + ctx->text_size);


    svg_finish(ctx);
    if (html)
    {
        fprintf(f, "</body>\n");
        fprintf(f, "</html>");
    }
}

void svg_draw_to_file_y(FILE *f, const char *color,
                        double *y, size_t count,
                        const char *xlabel, const char *ylabel,
                        int decimal, bool html)
{
    double *x = (double*) malloc(sizeof (double)*count);
    size_t i;
    for (i = 0; i < count; ++i)
        x[i] = i;
    svg_draw_to_file_xy(f, color, y, x, count, xlabel, ylabel, decimal, html);
    free(x);
}
