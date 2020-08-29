#include "src/jack.h"
#include "src/renderer.h"

void loop(float delta_time);
void init();

int main()
{
    if (!j_init()) return -1;

    init();

    j_loop_func(loop);
    return 0;
}

texture t;

int count = 10;
float positions[10][2];

void init_batch();
void draw_batch();

void init()
{
    j_set_window_size(800, 600);

    t = create_texture("textures/avatar.png");

    j_vec2i wsize = j_get_window_size();

    for (int i = 0; i < count; i++)
    {
        positions[i][0] = j_rand_int(100, wsize.x);
        positions[i][1] = j_rand_int(100, wsize.y);
    }

    init_batch();
}

void camera_controller(float delta_time)
{
    float speed = 300.0f * delta_time;

    if (j_is_key_down(GLFW_KEY_W))
    {
        renderer_translate((vec2) { 0.0f, -speed });
    }
    else if (j_is_key_down(GLFW_KEY_S))
    {
        renderer_translate((vec2) { 0.0f, speed });
    }

    if (j_is_key_down(GLFW_KEY_D))
    {
        renderer_translate((vec2) { -speed, 0.0f });
    }
    else if (j_is_key_down(GLFW_KEY_A))
    {
        renderer_translate((vec2) { speed, 0.0f });
    }

    if (j_is_key_down(GLFW_KEY_Q))
    {
        renderer_zoom(0.99);
    }
    else if (j_is_key_down(GLFW_KEY_E))
    {
        renderer_zoom(1.01);
    }
}

int quadCount = 0;
void loop(float delta_time)
{
    j_clear(J_GRAY);

    camera_controller(delta_time);

    /* renderer_fill_quad( */
        /* (vec4) { 1.0f, 1.0f, 0.0f, 1.0f }, */
        /* (vec2) { 0.0f, 0.0f }, (vec2) { 20.0f, 20.0f }, 0 */
    /* ); */

    draw_batch();

    char title[42];
    int fps = 1 / delta_time;
    sprintf(title, "%d - Quads: %d", fps, quadCount);
    j_set_window_title(title);
}

shader batch_sh;
void init_batch()
{

    batch_sh = create_shader("src/shaders/batching.vert", "src/shaders/batching.frag");
    shader_bind(&batch_sh);
    int samplers[16];
    for (int i = 0; i < 16; i++)
    {
        samplers[i] = i;
    }
    shader_set_uniform_1iv(&batch_sh, "u_Textures", 16, samplers);
}

void draw_batch()
{
    mat4 view_proj;
    glm_mat4_mul(
        renderer_instance.projection_mat, renderer_instance.view_mat, view_proj
    );

    shader_bind(&batch_sh);
    shader_set_uniform_mat4(&batch_sh, "u_ViewProj", view_proj);
    batch_renderer_begin();

    quadCount = 0;

    int t_indx = 0;
    for (float y = -2000.0f; y < 2000.0f; y += 20.0f)
    {
        for (float x = -2000.0f; x < 2000.0f; x += 20.0f)
        {
            quadCount++;
            vec4 color = (vec4) { (x + 2000) / 4000, 0.6f, (y + 2000) / 4000, 1.0f };
            batch_renderer_fill_quad((vec2) { x, y }, (vec2) { 20, 20 }, color);
            /* batch_renderer_textured_quad((vec2) { x, y }, (vec2) { 18, 18 }, t.renderer_id); */
        }
    }

    batch_renderer_end();
    batch_renderer_flush();
}
