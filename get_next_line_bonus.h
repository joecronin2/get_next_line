#pragma once
#include <stddef.h>

typedef struct s_chunk
{
  char *data;
  size_t len;
  struct s_chunk *next;
} t_chunk;


size_t calc_chunks_len (t_chunk *chunks);
t_chunk *create_chunk (unsigned char *str, size_t len);
int append_chunk (t_chunk **chunks, t_chunk *chunk);
void free_chunks (t_chunk *chunks);
