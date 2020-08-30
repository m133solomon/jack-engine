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

void renderer_translate(vec2 amount);
void renderer_zoom(float amount);

// rotation in radians
void renderer_draw_quad(vec2 translation, vec2 scale, float rotation, shader *sh);
void renderer_fill_quad(vec4 color, vec2 translation, vec2 scale, float rotation);
void renderer_textured_quad(texture *tex, vec4 tint, vec2 translation, vec2 scale, float rotation);
void renderer_clean();

// --- BATCH RENDERER ---

void batch_renderer_init();

void batch_renderer_begin();
void batch_renderer_end();

void batch_renderer_fill_quad(vec2 position, vec2 size, float rotation, vec4 color);
void batch_renderer_textured_quad(vec2 position, vec2 size, float rotation, uint32_t texture_id);

#endif // RENDERER_H
