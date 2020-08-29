#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>

typedef struct
{
    uint32_t renderer_id;
} vertex_buffer;

vertex_buffer create_vertex_buffer(const void* data, uint32_t size);
void vertex_buffer_bind(vertex_buffer *vb);
void vertex_buffer_unbind();
void vertex_buffer_delete(vertex_buffer *vb);

#endif // VERTEX_BUFFER_H
