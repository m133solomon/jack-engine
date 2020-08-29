#include "color.h"

j_colorf j_color_normalize(j_color *color)
{
    return (j_colorf) { color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f };
}