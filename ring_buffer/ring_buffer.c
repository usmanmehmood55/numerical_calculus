#include "ring_buffer.h"

void ring_buffer_init(ring_buffer *rb, uint16_t size)
{
    if (size == 0)
    {
        return; // Error: buffer size is zero
    }

    rb->buffer = NULL;
    rb->buffer = malloc(sizeof(double) * size);
    if (rb->buffer == NULL)
    {
        return;
    }

    memset(rb->buffer, 0, size * sizeof(double));
    rb->size  = size;
    rb->index = 0;
}

void ring_buffer_add(ring_buffer *rb, double item)
{
    rb->buffer[rb->index] = item;
    rb->index = (rb->index + 1) % rb->size;
}

double ring_buffer_get_item(const ring_buffer *rb, int16_t index)
{
    // to handle -1 index
    if (index < 0)
    {
        return 0.0;
    }

    // to handle overflow of index
    else if (index >= rb->size)
    {
        return rb->buffer[rb->size];
    }

    // normal case
    else
    {
        return rb->buffer[index];
    }
}

void print_buffer(const ring_buffer *rb)
{
    printf("\rBuffer Contents: {");
    for (uint16_t i = 0; i < rb->size; i++)
    {
        printf("%.3f, ", rb->buffer[i]);
    }
    printf("}\n");
}

void ring_buffer_destroy(ring_buffer *rb)
{
    free(rb->buffer);
    rb->size = 0;
    rb->index = 0;
}