#pragma once

typedef struct logl_shader logl_shader;

logl_shader *logl_shader_init();
void logl_shader_destroy(logl_shader *shader);

int logl_shader_load_vertex(logl_shader *shader, const char *vertex_file);
int logl_shader_load_fragment(logl_shader *shader, const char *fragment_file);

int logl_shader_link(logl_shader *shader);
int logl_shader_use(logl_shader *shader);

int logl_shader_uniform_set_4f(logl_shader *shader, const char *name, float v0, float v1, float v2, float v3);
int logl_shader_uniform_set_1f(logl_shader *shader, const char *name, float v0);
