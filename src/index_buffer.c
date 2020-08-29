#include "index_buffer.h"

#include <GL/glew.h>

index_buffer create_index_buffer(const unsigned int *data, unsigned int count)
{
    index_buffer ib;
    ib.count = count;
    glGenBuffers(1, &ib.renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return ib;
}

void index_buffer_bind(index_buffer *ib)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->renderer_id);
}

void index_buffer_unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer_delete(index_buffer *ib)
{
    glDeleteBuffers(1, &ib->renderer_id);
}

