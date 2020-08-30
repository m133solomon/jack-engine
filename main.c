#include "src/jack.h"
#include "src/renderer.h"
#include "math.h"

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

void init()
{
    j_set_window_size(800, 600);

    t = create_texture("textures/avatar.png");
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

    if (j_is_key_down(GLFW_KEY_R))
    {
        renderer_rotate(0.01f);
    }
    else if (j_is_key_down(GLFW_KEY_F))
    {
        renderer_rotate(-0.01f);
    }
}

int quadCount = 0;
void loop(float delta_time)
{
    j_clear(J_BLACK);

    camera_controller(delta_time);

    quadCount = 0;

    batch_renderer_begin();
    int t_indx = 0;
    for (float y = -2000.0f; y < 2000.0f; y += 30.0f)
    {
        for (float x = -2000.0f; x < 2000.0f; x += 30.0f)
        {
            quadCount++;
            vec4 color = (vec4) { (x + 2000) / 4000, 0.2f, (y + 2000) / 4000, 1.0f };
            /* batch_renderer_fill_quad((vec2) { x, y }, (vec2) { 15.0f, 15.0f }, 0, color); */
            batch_renderer_textured_quad((vec2) { x, y }, (vec2) { 30.0f, 30.0f }, 0.0f, t);
        }
    }
    batch_renderer_end();

    char title[42];
    int fps = 1 / delta_time;
    sprintf(title, "%d - Quads: %d", fps, quadCount);
    j_set_window_title(title);
}
