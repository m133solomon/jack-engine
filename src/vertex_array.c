#include "vertex_array.h"

vb_layout create_vb_layout()
{
    vb_layout result;

    result.stride = 0;
    result.elements = (vb_element*)malloc(1000 * sizeof(vb_layout));
    result.element_count = 0;

    return result;
}

void vb_layout_push_float(vb_layout *layout, uint32_t count)
{
    layout->elements[layout->element_count++] =
        (vb_element) { GL_FLOAT, count, GL_FALSE };
    layout->stride += count * sizeof(GL_FLOAT);
}

void vb_layout_push_uint(vb_layout *layout, uint32_t count)
{
    layout->elements[layout->element_count++] =
        (vb_element) { GL_UNSIGNED_INT, count, GL_FALSE };
    layout->stride += count * sizeof(GL_UNSIGNED_INT);
}

void vb_layout_delete(vb_layout *layout)
{
    free(layout->elements);
}

vertex_array create_vertex_array()
{
    vertex_array result;
    glGenVertexArrays(1, &result.renderer_id);
    return result;
}

void vertex_array_delete(vertex_array *va)
{
    glDeleteVertexArrays(1, &va->renderer_id);
}

void vertex_array_add_buffer(vertex_array *va, vertex_buffer vb, vb_layout layout)
{
    vertex_array_bind(va);
    vertex_buffer_bind(&vb);
    glEnableVertexAttribArray(0);

    uint32_t offset;

    for (uint32_t i = 0; i < layout.element_count; i++)
    {
        vb_element element = layout.elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, element.count, element.type, element.normalized,
            layout.stride, (const void*) offset
        );
        offset += element.count * sizeof(element.type);
    }
}

void vertex_array_bind(vertex_array *va)
{
    glBindVertexArray(va->renderer_id);
}

void vertex_array_unbind()
{
    glBindVertexArray(0);
}
