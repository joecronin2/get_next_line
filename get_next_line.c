#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

// aaaaa | aa\naa
//
// read 1 into static buffer
//
// 1 st  | 2 file
// aaaaa | aa\naa
//
// No \n, so copy into malloc 1
//
// 1 he  | 2 file
// aaaaa | aa\naa
//
// read 2 into static
// 1 he  | 2 st
// aaaaa | aa\naa
//
// \n idx 2
// join 2st to idx to he

int
append_chunk (t_chunk **chunks, char *buf)
{
  t_chunk *chunk = malloc (sizeof (*chunk));
  if (!chunk) return 0;
  chunk->data = malloc (BUFFER_SIZE);
  ft_memcpy (chunk->data, buf, BUFFER_SIZE);
  chunk->next = NULL;
  if (!*chunks)
    *chunks = chunk;
  else
    {
      t_chunk *tmp = *chunks;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = chunk;
    }
  return 1;
}

int
chunks_len (t_chunk *chunks)
{
  int len = 0;
  while (chunks)
    {
      chunks = chunks->next;
      len++;
    }
  return len;
}

// index in last chunk
char *
concat_chunks (t_chunk *chunks, size_t start_index, int last_size)
{
  int n_chunks = chunks_len (chunks);
  char *buf = malloc (((n_chunks - 1) * BUFFER_SIZE) + last_size + 1);
  if (!buf) return NULL;
  size_t i = 0;
  if (start_index)
    {
      ft_memcpy (buf, chunks->data + start_index, BUFFER_SIZE - start_index);
      i += BUFFER_SIZE - start_index;
      chunks = chunks->next;
    }
  while (chunks->next)
    {
      ft_memcpy (&buf[i], chunks->data, BUFFER_SIZE);
      chunks = chunks->next;
      i += BUFFER_SIZE;
    }
  ft_memcpy (&buf[i], chunks->data, last_size);
  buf[i + last_size] = '\0';
  return buf;
}

void
free_chunks (t_chunk *chunks)
{
  while (chunks)
    {
      t_chunk *tmp = chunks->next;
      free (chunks->data);
      free (chunks);
      chunks = tmp;
    }
}

char *
get_next_line (int fd)
{
  static char stash[BUFFER_SIZE];
  static size_t stash_end = 0;
  static size_t stash_start = 0;
  t_chunk *chunks = NULL;
  char *newline = NULL;
  size_t chunk_start = stash_start;
  if (stash_start < stash_end) append_chunk (&chunks, &stash[stash_start]);
  while (1)
    {
      newline = memchr (&stash[stash_start], '\n', stash_end - stash_start);
      if (newline)
        {
          stash_start = (newline - stash) + 1;
          break;
        }
      ssize_t n_read = read (fd, stash, BUFFER_SIZE);
      if (n_read <= 0)
        {
          stash_start = 0;
          stash_end = 0;
          break;
        }
      stash_start = 0;
      stash_end = n_read;
      chunk_start = 0;
      append_chunk (&chunks, stash);
    }
  size_t line_len = stash_start;
  char *line = concat_chunks (chunks, 0, line_len);
  free_chunks (chunks);
  return line;
}

#include <fcntl.h>

void
test_concat ()
{
  t_chunk *c = NULL;
  append_chunk (&c, "a123456789ABCDE ");
  append_chunk (&c, "b123456789ABCDE ");
  append_chunk (&c, "c123\n0000000000e");
  char *s = concat_chunks (c, 0, 3);
  int *a = malloc (4);
}

int
main ()
{
  // test_concat ();
  int fd = open ("./testfile", O_RDONLY);
  char *l;
  while ((l = get_next_line (fd)))
    {
      printf ("%s", l);
      free (l);
    }
  close (fd);
  return 0;
}
