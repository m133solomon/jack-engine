#include "jack.h"
#include "GLFW/glfw3.h"
#include "renderer.h"
#include <time.h>

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

j_app j_instance;

static void glfw_error_callback(int error, const char *description)
{
    LOG_ERROR("OpenGL Error: %s", description);
}

static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        j_instance.key_callback(key);
    }
}

static void glfw_resize_callback(GLFWwindow *window, int width, int height)
{
    j_vec2i new_size = (j_vec2i) { width, height };
    j_instance.window_size = new_size;
    renderer_update_viewport(new_size);
}

bool j_init()
{
    srand(time(NULL));

    j_instance = (j_app)
    {
        .window = NULL,
        .window_title = "jack app",
        .window_size = (j_vec2i) { 600, 400 },
        .window_vsync = false,
        .current_time = 0,
        .last_time = 0
    };

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        LOG_ERROR("Could not init glfw");
        return false;
    }

    // set opengl version to 4.1
    // and use core profile for macos to be happy
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // make window
    j_instance.window = glfwCreateWindow(
        j_instance.window_size.x, j_instance.window_size.y,
        j_instance.window_title, NULL, NULL
    );

    glfwSetFramebufferSizeCallback(j_instance.window, glfw_resize_callback);

    if (!j_instance.window)
    {
        glfwTerminate();
        LOG_ERROR("Could not create window");
        return false;
    }

    glfwMakeContextCurrent(j_instance.window);

    j_set_vsync(j_instance.window_vsync);

    // init glew
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        LOG_ERROR("Could not init glew");
        glfwTerminate();
        return false;
    }

    // set blending enabled by default
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // used for delta time
    j_instance.current_time = glfwGetTime();
    j_instance.last_time = j_instance.current_time;

    renderer_init(j_instance.window_size);

    return true;
}

void j_set_vsync(bool value)
{
    int v = value ? 1 : 0;
    glfwSwapInterval(v);
}

void j_set_window_title(char *title)
{
    j_instance.window_title = title;
    glfwSetWindowTitle(j_instance.window, title);
}

char *j_get_window_title()
{
    return j_instance.window_title;
}

void j_set_window_size(uint32_t w, uint32_t h)
{
    j_instance.window_size = (j_vec2i) { w, h };
    glfwSetWindowSize(j_instance.window, w, h);
    renderer_update_viewport(j_instance.window_size);
}

j_vec2i j_get_window_size()
{
    return j_instance.window_size;
}

static void j_clean()
{
    renderer_clean();
    glfwTerminate();
}

void j_clear(j_color clear_color)
{
    j_colorf normalized = j_color_normalize(&clear_color);
    glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void j_loop_func(void (*loop_func)(float delta_time))
{
    while (!glfwWindowShouldClose(j_instance.window))
    {
        // calculate delta time
        // (the time difference between the last and current frame)
        j_instance.current_time = glfwGetTime();
        float delta_time = j_instance.current_time - j_instance.last_time;
        j_instance.last_time = j_instance.current_time;

        loop_func(delta_time);

        glfwSwapBuffers(j_instance.window);
        glfwPollEvents();
    }
    j_clean();
}

void j_set_key_press_callback(void (*key_callback)(int key))
{
    j_instance.key_callback = key_callback;
    glfwSetKeyCallback(j_instance.window, glfw_key_callback);
}

bool j_is_key_down(int key)
{
    if (glfwGetKey(j_instance.window, key))
    {
        return true;
    }
    return false;
}
