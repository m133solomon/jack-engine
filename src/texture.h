#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>

typedef struct
{
    uint32_t renderer_id;
    unsigned char *local_buffer;
    int width;
    int height;
    int bits_per_pixel;
} texture;

texture create_texture(char *filepath);
void texture_delete(texture *texture);
void texture_bind(texture *texture, uint32_t slot);
void texture_unbind();
void texture_delete(texture *texture);

#endif // TEXTURE_H
