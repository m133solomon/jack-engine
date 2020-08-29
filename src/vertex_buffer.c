#include "vertex_buffer.h"

vertex_buffer create_vertex_buffer(const void *data, uint32_t size)
{
    vertex_buffer result;

    glGenBuffers(1, &result.renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, result.renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return result;
}

void vertex_buffer_bind(vertex_buffer *vb)
{
    glBindBuffer(GL_ARRAY_BUFFER, vb->renderer_id);
}

void vertex_buffer_unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer_delete(vertex_buffer *vb)
{
    glDeleteBuffers(1, &vb->renderer_id);
}
