#ifndef JACK_H
#define JACK_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "log.h"
#include "color.h"
#include "math.h"

typedef struct
{
    GLFWwindow *window;
    char *window_title;
    j_vec2i window_size;
    bool window_vsync;
    void (*key_callback)(int key);

    // used for delta time
    double current_time;
    double last_time;
} j_app;

// declare this as extern so
// otherfile can forward declare it
extern j_app j_instance;

bool j_init();

void j_set_vsync(bool value);

void j_set_window_title(char *title);
char* j_get_window_title();

void j_set_window_size(uint32_t w, uint32_t h);
j_vec2i j_get_window_size();

static void j_clean();

void j_clear(j_color clear_color);

// application will start after this func;
void j_loop_func(void (*loop_func)(float delta_time));

void j_set_key_press_callback(void (*key_callback)(int key));
bool j_is_key_down(int key);

// (TODO): implement these
void j_fill_rect();
void j_stroke_rect();
void j_draw_image();
// ----

#endif // j_H
