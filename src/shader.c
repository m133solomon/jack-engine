#include "shader.h"
#include "log.h"
#include "unistd.h"

static char* read_file_source(const char *filepath)
{
    if (access(filepath, F_OK) == -1)
    {
        LOG_ERROR("Shader source \"%s\" does not exist", filepath);
        // exit the program if the files does not exist
        exit(-1);
    }

    // https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
    // find file length
    // declare string that contains shader source
    // make it file_length + 1 because
    // fread does not add the string termination
    // character \0

    FILE *shader_file = fopen(filepath, "rt");

    fseek(shader_file, 0L, SEEK_END);
    long int file_length = ftell(shader_file);
    fseek(shader_file, 0L, SEEK_SET);

    char *shader_source = (char*)malloc(file_length + 1);

    fread(shader_source, 1, file_length + 1, shader_file);
    shader_source[file_length] = '\0';

    fclose(shader_file);

    return shader_source;
}

static uint32_t shader_compile(const uint32_t type, const char *source)
{
    uint32_t id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length + 1];
        glGetShaderInfoLog(id, length, &length, message);
        LOG_ERROR("Failed to compile shader: \n%s\n", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

shader create_shader(const char *vertex_filepath, const char *fragment_filepath)
{
    shader result;

    char *vertex_source = read_file_source(vertex_filepath);
    char *fragment_source = read_file_source(fragment_filepath);

    uint32_t program = glCreateProgram();

    uint32_t vertex_shader = shader_compile(GL_VERTEX_SHADER, vertex_source);
    uint32_t fragment_shader = shader_compile(GL_FRAGMENT_SHADER, fragment_source);

    free(vertex_source);
    free(fragment_source);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    result.renderer_id = program;

    return result;
}

void shader_delete(shader *shader)
{
    glDeleteProgram(shader->renderer_id);
}

void shader_bind(shader *shader)
{
    glUseProgram(shader->renderer_id);
}

void shader_unbind()
{
    glUseProgram(0);
}

int shader_get_uniform_location(shader *shader, char *uniform_name)
{
    int location = glGetUniformLocation(shader->renderer_id, uniform_name);
    if (location == -1)
    {
        LOG_WARN("No uniform: %s", uniform_name);
    }
    return location;
}

void shader_set_uniform1f(shader *shader, char *uniform_name, float a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform1f(uniform_location, a);
}

void shader_set_uniform4f(shader *shader, char *uniform_name, float a, float b, float c, float d)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform4f(uniform_location, a, b, c, d);
}

void shader_set_uniform1i(shader *shader, char *uniform_name, int a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform1i(uniform_location, a);
}

void shader_set_uniform_vec2(shader *shader, char *uniform_name, vec2 a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform2f(uniform_location, a[0], a[1]);
}

void shader_set_uniform_vec3(shader *shader, char *uniform_name, vec3 a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform3f(uniform_location, a[0], a[1], a[2]);
}

void shader_set_uniform_vec4(shader *shader, char *uniform_name, vec4 a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform4f(uniform_location, a[0], a[1], a[2], a[3]);
}

void shader_set_uniform_mat4(shader *shader, char *uniform_name, mat4 a)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, a[0]);
}

void shader_set_uniform_1iv(shader *shader, char *uniform_name, int count, int *data)
{
    int uniform_location = shader_get_uniform_location(shader, uniform_name);
    glUniform1iv(uniform_location, count, data);
}
