# Indicate the source files
set(LIB_SRCS ring_buffer.h  ring_buffer.c)

# Set the library type as static
SET (LIB_TYPE STATIC)

add_library (ring_buffer ${LIB_TYPE} ${LIB_SRCS})