#pragma once

#include <stddef.h>

#define BUFFER_SIZE 16

typedef struct s_chunk
{
  char *data;
  size_t len;
  struct s_chunk *next;
} t_chunk;

char *get_next_line (int fd);
void *ft_memchr (const void *s, int c, size_t n);
void *ft_memcpy (void *dest, const void *src, size_t n);
size_t ft_strlen (const char *s);
char *ft_substr (char const *s, unsigned int start, size_t len);
