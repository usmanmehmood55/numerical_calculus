#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    double * buffer;
    uint16_t size;
    uint16_t index;
} ring_buffer;

void ring_buffer_init(ring_buffer *rb, uint16_t size);

void ring_buffer_add(ring_buffer *rb, double item);

double ring_buffer_get_item(const ring_buffer *rb, int16_t index);

void print_buffer(const ring_buffer *rb);

void ring_buffer_destroy(ring_buffer *rb);

#endif