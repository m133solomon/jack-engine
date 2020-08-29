#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} j_color;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} j_colorf;

j_colorf j_color_normalize(j_color *color);

#define J_BLACK (j_color) { 0, 0, 0, 255 }
#define J_WHITE (j_color) { 255, 255, 255, 255 }
#define J_GRAY (j_color) { 50, 50, 50, 255 }
#define J_RED (j_color) { 255, 0, 0, 255 }
#define J_BLUE (j_color) { 0, 0, 255, 255 }
#define J_GREEN (j_color) { 0, 255, 0, 255 }

#endif // COLOR_H