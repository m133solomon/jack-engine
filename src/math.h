#ifndef MATH_H
#define MATH_H

typedef struct
{
    int x;
    int y;
} j_vec2i;

typedef struct
{
    float x;
    float y;
} j_vec2f;

int j_rand_int(int min_num, int max_num);
float j_rand_float(float min_num, float max_num);

#endif // MATH_H
