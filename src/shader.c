#include "shader.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <glad/gl.h>

#include "file.h"

void check_shader_compile_errors(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
    }
}

void check_program_link_errors(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::LINK_FAILED\n%s", infoLog);
    }
}

struct logl_shader
{
    unsigned int program;

    char *vertex_file;
    char *vertex_code;

    char *fragment_file;
    char *fragment_code;
};

int logl_shader_load_vertex(logl_shader *shader, const char *vertex_file)
{
    size_t len = strlen(vertex_file) + 1;

    shader->vertex_file = malloc(len);

    if (strcpy_s(shader->vertex_file, len, vertex_file))
        return 1;

    free(shader->vertex_code);
    return logl_file_read_text(shader->vertex_file, &shader->vertex_code);
}

int logl_shader_load_fragment(logl_shader *shader, const char *fragment_file)
{
    size_t len = strlen(fragment_file) + 1;

    shader->fragment_file = malloc(len);

    if (strcpy_s(shader->fragment_file, len, fragment_file))
        return 1;

    free(shader->fragment_code);
    return logl_file_read_text(shader->fragment_file, &shader->fragment_code);
}

unsigned int logl_shader_compile_vertex(logl_shader *shader)
{
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &shader->vertex_code, NULL);
    glCompileShader(vertex_shader);

    check_shader_compile_errors(vertex_shader);

    return vertex_shader;
}

unsigned int logl_shader_compile_fragment(logl_shader *shader)
{
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader, 1, &shader->fragment_code, NULL);
    glCompileShader(fragment_shader);

    check_shader_compile_errors(fragment_shader);

    return fragment_shader;
}

int logl_shader_link(logl_shader *shader)
{
    unsigned int vertex_shader = 0;

    // create program
    if (shader->program != 0)
        glDeleteProgram(shader->program);

    shader->program = glCreateProgram();

    // compile shaders
    if (shader->vertex_code != NULL)
    {
        vertex_shader = logl_shader_compile_vertex(shader);
        glAttachShader(shader->program, vertex_shader);
    }

    unsigned int fragment_shader = 0;

    if (shader->fragment_code != NULL)
    {
        fragment_shader = logl_shader_compile_fragment(shader);
        glAttachShader(shader->program, fragment_shader);
    }

    // link program
    glLinkProgram(shader->program);

    check_program_link_errors(shader->program);

    // delete shaders
    if (vertex_shader != 0)
        glDeleteShader(vertex_shader);

    if (fragment_shader != 0)
        glDeleteShader(fragment_shader);

    return shader->program == 0 ? 1 : 0;
}

int logl_shader_use(logl_shader *shader)
{
    assert(shader->program != 0);

    glUseProgram(shader->program);

    return 0;
}

int logl_shader_uniform_set_4f(logl_shader *shader, const char *name, float v0, float v1, float v2, float v3)
{
    assert(shader->program != 0);

    glUniform4f(glGetUniformLocation(shader->program, name), v0, v1, v2, v3);

    return 0;
}

int logl_shader_uniform_set_1f(logl_shader *shader, const char *name, float v0)
{
    assert(shader->program != 0);

    glUniform1f(glGetUniformLocation(shader->program, name), v0);

    return 0;
}

logl_shader *logl_shader_init()
{
    logl_shader *shader = malloc(sizeof(logl_shader));

    shader->program = 0;

    shader->vertex_file = NULL;
    shader->vertex_code = NULL;

    shader->fragment_file = NULL;
    shader->fragment_code = NULL;

    return shader;
}

void logl_shader_destroy(logl_shader *shader)
{
    free(shader->vertex_file);
    free(shader->vertex_code);

    free(shader->fragment_file);
    free(shader->fragment_code);

    if (shader->program != 0)
        glDeleteProgram(shader->program);

    free(shader);
}
