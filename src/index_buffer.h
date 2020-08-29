#ifndef index_BUFFER_H
#define index_BUFFER_H

typedef struct
{
    unsigned int renderer_id;
    unsigned int count;
} index_buffer;

index_buffer create_index_buffer(const unsigned int *data, unsigned int count);
void index_buffer_bind(index_buffer *ib);
void index_buffer_unbind();
void index_buffer_delete(index_buffer *ib);

#endif // index_BUFFER_H

