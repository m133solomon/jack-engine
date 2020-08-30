#include "renderer.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "log.h"
#include <stdint.h>
#include <stdlib.h>

/*
 *    FRIENDLY REMINDER
 *
 *    x * mat4 != mat4 * x
 *    stupid dog
 */

typedef struct
{
    j_vec2i render_size;

    vertex_array quad_va;
    index_buffer quad_ib;
    shader quad_texture_shader;
    shader quad_color_shader;

    mat4 projection_mat;
    mat4 view_mat;

    j_vec2f translation;
    float zoom_amount;
    // radians
    float rotation;
} j_renderer;

j_renderer renderer_instance;

static void init_quad_rendering()
{
    // declaring vertices for the quad
    // containing positions and tex coords
    float quad_vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    // declare the indices for the vertices
    unsigned int quad_indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // create a vao to keep the state
    // (vertex buffer, index buffer)
    renderer_instance.quad_va = create_vertex_array();

    vertex_buffer quad_vb = create_vertex_buffer(&quad_vertices, sizeof(quad_vertices));
    vb_layout quad_layout = create_vb_layout();
    vb_layout_push_float(&quad_layout, 2);
    vb_layout_push_float(&quad_layout, 2);
    vertex_array_add_buffer(&renderer_instance.quad_va, quad_vb, quad_layout);

    renderer_instance.quad_ib = create_index_buffer(quad_indices, 6);

    // TODO: move these shaders to a string
    // make additional methods for loading
    // shaders from strings or files

    // create the texture shader for the quad
    renderer_instance.quad_texture_shader = create_shader(
        "src/shaders/texture.vert", "src/shaders/texture.frag"
    );

    // create the color shader
    renderer_instance.quad_color_shader = create_shader(
        "src/shaders/color.vert", "src/shaders/color.frag"
    );

    vb_layout_delete(&quad_layout);
}

void renderer_update_viewport(j_vec2i size)
{
    renderer_instance.render_size = size;

    glm_ortho(
        -size.x / 2 * renderer_instance.zoom_amount,
        size.x / 2 * renderer_instance.zoom_amount,
        -size.y / 2 * renderer_instance.zoom_amount,
        size.y / 2 * renderer_instance.zoom_amount,
        -1.0f, 1.0f,
       renderer_instance.projection_mat
    );
}

void renderer_init(j_vec2i size)
{
    renderer_instance.translation = (j_vec2f) { 0.0f, 0.0f };
    renderer_instance.zoom_amount = 1.0f;
    renderer_instance.rotation = 0;

    renderer_update_viewport(size);

    // view matrix (camera)
    glm_mat4_identity(renderer_instance.view_mat);

    init_quad_rendering();

    batch_renderer_init();
}

void renderer_draw(vertex_array *va, index_buffer *ib, shader *sh)
{
    shader_bind(sh);
    vertex_array_bind(va);

    glDrawElements(GL_TRIANGLES, ib->count, GL_UNSIGNED_INT, NULL);
}

void renderer_translate(vec2 amount)
{
    renderer_instance.translation = j_vec2f_add(
        renderer_instance.translation,
        (j_vec2f) { amount[0], amount[1] }
    );

    glm_translate(
        renderer_instance.view_mat, (vec3) { amount[0], amount[1], 0.0f }
    );
}

// this multiplies the zoom amount
void renderer_zoom(float amount)
{
    renderer_instance.zoom_amount *= amount;
    renderer_update_viewport(renderer_instance.render_size);
}

// TODO: fix this ortho rotation shit
void renderer_rotate(float amount)
{
    renderer_instance.rotation += amount;
    glm_rotate_z(
        renderer_instance.view_mat, amount, renderer_instance.view_mat
    );
    renderer_update_viewport(renderer_instance.render_size);
}

j_vec2f renderer_get_translation()
{
    return renderer_instance.translation;
}

float renderer_get_zoom_amount()
{
    return renderer_instance.zoom_amount;
}

float renderer_get_rotation()
{
    return renderer_instance.zoom_amount;
}

void renderer_draw_quad(vec2 translation, vec2 scale, float rotation, shader *sh)
{
    // crate the model matrix for this specific quad
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) { translation[0], translation[1], 0.0f });
    glm_scale(model, (vec3) { scale[0], scale[1], 1.0f });
    glm_rotate_z(model, rotation, model);

    // mvp -> MODEL VIEW PROJECTION
    // the result from combining all the 3 matrices
    mat4 mvp;
    mat4 view_proj;
    glm_mat4_mul(
        renderer_instance.projection_mat, renderer_instance.view_mat, view_proj
    );
    glm_mat4_mul(view_proj, model, mvp);

    // bind the shader and set the mvp
    shader_bind(sh);
    shader_set_uniform_mat4(sh, "u_MVP", mvp);

    renderer_draw(&renderer_instance.quad_va, &renderer_instance.quad_ib, sh);
}

void renderer_fill_quad(vec4 color, vec2 translation, vec2 scale, float rotation)
{
    shader_bind(&renderer_instance.quad_color_shader);
    shader_set_uniform_vec4(
        &renderer_instance.quad_color_shader,
        "u_Color", color
    );

    renderer_draw_quad(translation, scale, rotation, &renderer_instance.quad_color_shader);
}

void renderer_textured_quad(texture *tex, vec4 tint, vec2 translation, vec2 scale, float rotation)
{
    texture_bind(tex, 0);
    shader_bind(&renderer_instance.quad_texture_shader);
    shader_set_uniform1i(&renderer_instance.quad_texture_shader, "u_Texture", 0);
    shader_set_uniform_vec4(
        &renderer_instance.quad_texture_shader, "u_Color", tint
    );

    renderer_draw_quad(translation, scale, rotation, &renderer_instance.quad_texture_shader);
}

// --- BATCH RENDERER ---

static const uint32_t MAX_QUAD_COUNT = 10000;
static const uint32_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
static const uint32_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
static int MAX_TEXTURES;

typedef struct
{
    vec3 position;
    vec4 color;
    vec2 tex_coords;
    float tex_index;
} vertex;

typedef struct
{
    vec3 quad_vertex_positions[4];
    vec2 quad_texture_coords[4];

    GLuint quad_va;
    GLuint quad_vb;
    GLuint quad_ib;

    shader quad_shader;

    GLuint white_texture;
    uint32_t white_texture_slot;

    uint32_t index_count;

    vertex* quad_buffer;
    vertex* quad_buffer_ptr;

    uint32_t *texture_slots;
    uint32_t texture_slot_index;
} batch_renderer_data;

static batch_renderer_data renderer_data;
static void init_renderer_data()
{
    // init vertex positions
    glm_vec3_copy(
        (vec3) { -0.5f, -0.5f, 0.0f },
        renderer_data.quad_vertex_positions[0]
    );
    glm_vec3_copy(
        (vec3) { 0.5f, -0.5f, 0.0f },
        renderer_data.quad_vertex_positions[1]
    );
    glm_vec3_copy(
        (vec3) { 0.5f, 0.5f, 0.0f },
        renderer_data.quad_vertex_positions[2]
    );
    glm_vec3_copy(
        (vec3) { -0.5f, 0.5f, 0.0f },
        renderer_data.quad_vertex_positions[3]
    );

    // init texture coords
    glm_vec2_copy(
        (vec2) { 0.0f, 0.0f },
        renderer_data.quad_texture_coords[0]
    );
    glm_vec2_copy(
        (vec2) { 1.0f, 0.0f },
        renderer_data.quad_texture_coords[1]
    );
    glm_vec2_copy(
        (vec2) { 1.0f, 1.0f },
        renderer_data.quad_texture_coords[2]
    );
    glm_vec2_copy(
        (vec2) { 0.0f, 1.0f },
        renderer_data.quad_texture_coords[3]
    );

    renderer_data.quad_va = 0;
    renderer_data.quad_vb = 0;
    renderer_data.quad_ib = 0;

    renderer_data.quad_shader = create_shader(
        "src/shaders/batching.vert", "src/shaders/batching.frag"
    );

    shader_bind(&renderer_data.quad_shader);
    int samplers[16];
    for (int i = 0; i < 16; i++)
    {
        samplers[i] = i;
    }
    shader_set_uniform_1iv(&renderer_data.quad_shader, "u_Textures", 16, samplers);

    renderer_data.white_texture = 0;
    renderer_data.white_texture_slot = 0;

    renderer_data.index_count = 0;

    renderer_data.quad_buffer = (vertex*)malloc(sizeof(vertex) * MAX_VERTEX_COUNT);

    renderer_data.texture_slots = (uint32_t*)malloc(MAX_TEXTURES * sizeof(uint32_t));
    renderer_data.texture_slot_index = 0;
}

void batch_renderer_init()
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);

    init_renderer_data();

    glGenVertexArrays(1, &renderer_data.quad_va);
    glBindVertexArray(renderer_data.quad_va);

    glGenBuffers(1, &renderer_data.quad_vb);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_data.quad_vb);
    glBufferData(
        GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vertex), NULL, GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);
    /* glEnableVertexArrayAttrib(renderer_data.quad_va, 0); */
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
        (const void*)offsetof(vertex, position)
    );

    glEnableVertexAttribArray(1);
    /* glEnableVertexArrayAttrib(renderer_data.quad_va, 1); */
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
        (const void*)offsetof(vertex, color)
    );

    glEnableVertexAttribArray(2);
    /* glEnableVertexArrayAttrib(renderer_data.quad_va, 2); */
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
        (const void*)offsetof(vertex, tex_coords)
    );

    glEnableVertexAttribArray(3);
    /* glEnableVertexArrayAttrib(renderer_data.quad_va, 3); */
    glVertexAttribPointer(
        3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex),
        (const void*)offsetof(vertex, tex_index)
    );

    uint32_t indices[MAX_INDEX_COUNT];
    uint32_t offset = 0;
    for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenBuffers(1, &renderer_data.quad_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_data.quad_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &renderer_data.white_texture);
    glBindTexture(GL_TEXTURE_2D, renderer_data.white_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color
    );

    renderer_data.texture_slots[0] = renderer_data.white_texture;

    for (uint32_t i = 1; i < MAX_TEXTURES; i++)
    {
        renderer_data.texture_slots[i] = 0;
    }
}

static void batch_renderer_shutdown()
{
    glDeleteVertexArrays(1, &renderer_data.quad_va);
    glDeleteBuffers(1, &renderer_data.quad_vb);
    glDeleteBuffers(1, &renderer_data.quad_ib);
    glDeleteTextures(1, &renderer_data.white_texture);

    shader_delete(&renderer_data.quad_shader);

    free(renderer_data.texture_slots);
    free(renderer_data.quad_buffer);
}

void batch_renderer_begin()
{
    mat4 view_proj;
    glm_mat4_mul(
        renderer_instance.projection_mat, renderer_instance.view_mat, view_proj
    );

    shader_bind(&renderer_data.quad_shader);
    shader_set_uniform_mat4(&renderer_data.quad_shader, "u_ViewProj", view_proj);

    renderer_data.quad_buffer_ptr = renderer_data.quad_buffer;
}

static void batch_renderer_flush()
{
    for (uint32_t i = 0; i < renderer_data.texture_slot_index; i++)
    {
       /* glBindTextureUnit(i, renderer_data.texture_slots[i]); */
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderer_data.texture_slots[i]);
    }

    glBindVertexArray(renderer_data.quad_va);
    glDrawElements(GL_TRIANGLES, renderer_data.index_count, GL_UNSIGNED_INT, NULL);

    renderer_data.index_count = 0;
    renderer_data.texture_slot_index = 1;
}


void batch_renderer_end()
{
    GLsizeiptr size = (uint8_t*)renderer_data.quad_buffer_ptr - (uint8_t*)renderer_data.quad_buffer;

    glBindBuffer(GL_ARRAY_BUFFER, renderer_data.quad_vb);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer_data.quad_buffer);

    batch_renderer_flush();
}

static void batch_renderer_bind_data(vec2 position, vec2 size, float rotation, vec4 color, float texture_index)
{
    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, (vec3) { position[0], position[1], 0.0f });
    glm_scale(transform, (vec3) { size[0], size[1], 0.0f });
    glm_rotate_z(transform, rotation, transform);

    // 4 vertices
    for (int i = 0; i < 4; i++)
    {
        vec3 position;
        glm_mat4_mulv3(transform, renderer_data.quad_vertex_positions[i], 1.0f, position);
        glm_vec3_copy(
            position,
            renderer_data.quad_buffer_ptr->position
        );
        glm_vec4_copy(color, renderer_data.quad_buffer_ptr->color);
        glm_vec2_copy(
            renderer_data.quad_texture_coords[i],
            renderer_data.quad_buffer_ptr->tex_coords
        );
        renderer_data.quad_buffer_ptr->tex_index = texture_index;
        renderer_data.quad_buffer_ptr++;
    }
    renderer_data.index_count += 6;
}

void batch_renderer_fill_quad(vec2 position, vec2 size, float rotation, vec4 color)
{
    if (renderer_data.index_count >= MAX_INDEX_COUNT)
    {
        batch_renderer_end();
        batch_renderer_flush();
        batch_renderer_begin();
    }

    float texture_index = 0.0f;
    batch_renderer_bind_data(position, size, rotation, color, texture_index);
}

void batch_renderer_textured_quad(vec2 position, vec2 size, float rotation, texture texture)
{
    float texture_id = texture.renderer_id;

    if (
        renderer_data.index_count >= MAX_INDEX_COUNT ||
        renderer_data.texture_slot_index > MAX_TEXTURES - 1
    )
    {
        batch_renderer_end();
        batch_renderer_flush();
        batch_renderer_begin();
    }

    vec4 color = (vec4) { 1.0f, 1.0f, 1.0f, 1.0f };

    float texture_index = 0.0f;

    for (uint32_t i = 1; i < renderer_data.texture_slot_index; i++)
    {
        if (renderer_data.texture_slots[i] == texture_id)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        texture_index = (float)renderer_data.texture_slot_index;
        renderer_data.texture_slots[renderer_data.texture_slot_index] = texture_id;
        renderer_data.texture_slot_index++;
    }

    batch_renderer_bind_data(position, size, rotation, color, texture_index);
}

void renderer_clean()
{
    shader_delete(&renderer_instance.quad_color_shader);
    shader_delete(&renderer_instance.quad_texture_shader);
    vertex_array_delete(&renderer_instance.quad_va);
    index_buffer_delete(&renderer_instance.quad_ib);
    batch_renderer_shutdown();
}

