#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    uint32_t renderer_id;
} shader;

shader create_shader(const char *vertex_filepath, const char *fragment_filepath);
void shader_delete(shader *shader);
void shader_bind(shader *shader);
void shader_unbind();

int shader_get_uniform_location(shader *shader, char *uniform_name);

void shader_set_uniform1f(shader *shader, char *uniform_name, float a);
void shader_set_uniform4f(shader *shader, char *uniform_name, float a, float b, float c, float d);
void shader_set_uniform1i(shader *shader, char *uniform_name, int a);
void shader_set_uniform_vec2(shader *shader, char *uniform_name, vec2 a);
void shader_set_uniform_vec3(shader *shader, char *uniform_name, vec3 a);
void shader_set_uniform_vec4(shader *shader, char *uniform_name, vec4 a);
void shader_set_uniform_mat4(shader *shader, char *uniform_name, mat4 a);
void shader_set_uniform_1iv(shader *shader, char *uniform_name, int count, int *data);

#endif // SHADER_H
