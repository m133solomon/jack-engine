#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "vertex_array.h"
#include "shader.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"

#include "math.h"

void renderer_update_viewport(j_vec2i size);
void renderer_init(j_vec2i size);
void renderer_draw(vertex_array *va, index_buffer *ib, shader *sh);

j_vec2f renderer_get_translation();
float renderer_get_zoom_amount();
float renderer_get_rotation();

void renderer_translate(vec2 amount);
void renderer_zoom(float amount);
void renderer_rotate(float amount);

void renderer_clean();

// --- BATCH RENDERER ---

void batch_renderer_init();

void batch_renderer_begin();
void batch_renderer_end();

void batch_renderer_fill_quad(vec2 position, vec2 size, float rotation, vec4 color);
void batch_renderer_textured_quad(vec2 position, vec2 size, float rotation, texture texture);

#endif // RENDERER_H
