#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

// vertex array and vertex buffer layout

#include <GL/glew.h>
#include <stdlib.h>

#include "vertex_buffer.h"

// VB -> vertex buffer

typedef struct
{
    uint32_t type;
    uint32_t count;
    unsigned char normalized;
} vb_element;

typedef struct
{
    vb_element *elements;
    uint32_t element_count;
    uint32_t stride;
} vb_layout;

vb_layout create_vb_layout();
void vb_layout_push_float(vb_layout *layout, uint32_t count);
void vb_layout_push_uint(vb_layout *layout, uint32_t count);
void vb_layout_push_element(vb_layout *layout, int index, const void* offset, vb_element element);

void vb_layout_delete(vb_layout *layout);

typedef struct {
    uint32_t renderer_id;
} vertex_array;

vertex_array create_vertex_array();
void vertex_array_delete(vertex_array *va);
void vertex_array_add_buffer(vertex_array *va, vertex_buffer vb, vb_layout layout);
void vertex_array_bind(vertex_array *va);
void vertex_array_unbind();

#endif // VERTEX_ARRAY_H

