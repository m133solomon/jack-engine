#include <GL/glew.h>
#include "texture.h"
#include "stb_image.h"

texture create_texture(char *filepath)
{
    texture result;

    result.local_buffer = NULL;
    result.width = 0;
    result.height = 0;
    result.bits_per_pixel = 0;

    stbi_set_flip_vertically_on_load(1);
    result.local_buffer = stbi_load(filepath, &result.width, &result.height, &result.bits_per_pixel, 4);

    glGenTextures(1, &result.renderer_id);
    glBindTexture(GL_TEXTURE_2D, result.renderer_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, result.width, result.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, result.local_buffer);

    texture_bind(&result, 0);

    if (result.local_buffer)
    {
        stbi_image_free(result.local_buffer);
    }

    return result;;
}

void texture_delete(texture *texture)
{
    glDeleteTextures(1, &texture->renderer_id);
}

void texture_bind(texture *texture, uint32_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->renderer_id);
}

void texture_unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
