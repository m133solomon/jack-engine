#include "math.h"
#include <stdlib.h>

j_vec2i j_vec2i_add(j_vec2i a, j_vec2i b)
{
    return (j_vec2i) { a.x + a.x, a.y + a.y };
}

j_vec2f j_vec2f_add(j_vec2f a, j_vec2f b)
{
    return (j_vec2f) { a.x + a.x, a.y + a.y };
}

int j_rand_int(const int min_num, const int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    }
    else
    {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

float j_rand_float(float min_num, float max_num)
{
    float scale = rand() / (float)RAND_MAX;
    return min_num + scale * (max_num - min_num);
}
