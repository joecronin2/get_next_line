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

t_chunk *
create_chunk (char *stash, size_t start, size_t len)
{
  t_chunk *chunk = malloc (sizeof (*chunk));
  if (!chunk) return 0;
  chunk->data = malloc (BUFFER_SIZE);
  if (!chunk->data)
    {
      free (chunk);
      return 0;
    }
  chunk->len = len;
  ft_memcpy (chunk->data, &stash[start], len);
  chunk->next = NULL;
  return chunk;
}

int
append_chunk (t_chunk **chunks, t_chunk *chunk)
{
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

size_t
calc_chunks_len (t_chunk *chunks)
{
  size_t count = 0;
  while (chunks)
    {
      count += chunks->len;
      chunks = chunks->next;
    }
  return count;
}

// index in last chunk
char *
concat_chunks (t_chunk *chunks)
{
  if (!chunks) return NULL;
  size_t len = calc_chunks_len (chunks);
  char *buf = malloc (len);
  if (!buf) return NULL;
  size_t i = 0;
  while (chunks)
    {
      ft_memcpy (&buf[i], chunks->data, chunks->len);
      i += chunks->len;
      chunks = chunks->next;
    }
  buf[i] = '\0';
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

// a$ab c$a$ abc$ abcd
// a, abc, a, abc, abcd
// start: 0 -> read stash
// memchr 1:
//  start = 2
// 	append chunk, return
// start: 2 -> use previous stash
// memchr NULL:
// 	start = 0
// 	append chunk, loop
// start: 0 -> read stash
// memchr 1:
//  start = 2
// 	append chunk, return
//
// abcd efgh ij$0
// abcdefghij
// start: 0 -> read stash
// memchr NULL:
// 	start = 0
// 	append chunk (memchr NULL so (0,BUFFER_SIZE), loop
// current chunks: abcd
// start: 0 -> read stash
// memchr NULL:
// 	start = 0
// 	append chunk (memchr NULL so (0,BUFFER_SIZE), loop
// current chunks: abcd, efgh
// start: 0 -> read stash
// memchr 2:
// 	start = 3
// 	append chunk (memchr 2 so (0,2), loop
// 	current chunks: abcd, efgh, ij
// 	memchr not null -> return chunks

// always add at most (BUFFER_SIZE - start) bytes to chunk
//
// cases:
// start, newline
//
//
// no start, newline
// no start, no newline
// start, no newline
//

char *
get_next_line (int fd)
{
  static char stash[BUFFER_SIZE];
  static size_t start = 0;
  t_chunk *chunks = NULL;
  while (1)
    {
      ssize_t n_bytes;
      if (start == 0)
        n_bytes = read (fd, stash, BUFFER_SIZE);
      else
        n_bytes = BUFFER_SIZE;

      if (n_bytes == 0) break;

      size_t last_start = start;
      char *newline = ft_memchr (&stash[start], '\n', n_bytes);
      if (newline)
        {
          size_t len = newline - stash + 1; // include newline
          // n_bytes >= len
          t_chunk *chunk = create_chunk (stash, last_start, len);
          start = len;
          append_chunk (&chunks, chunk);
          if (n_bytes < BUFFER_SIZE) start = 0;
          break;
        }
      else
        {
          size_t len = n_bytes - last_start;
          t_chunk *chunk = create_chunk (stash, last_start, len);
          append_chunk (&chunks, chunk);
          start = 0;
        }
    }
  char *line = concat_chunks (chunks);
  free_chunks (chunks);
  return line;
}

// char *
// get_next_line (int fd)
// {
//   static char stash[BUFFER_SIZE];
//   static size_t start = 0;
//   static size_t end = 0;
//   t_chunk *chunks = NULL;
//   char *newline = NULL;
//   size_t chunk_start = start;
//   if (start < end)
//     append_chunk (&chunks, &stash[start], end - start);
//   while (1)
//     {
//       newline = memchr (&stash[start], '\n', end - start);
//       if (newline)
//         {
//           start = (newline - stash) + 1;
//           break;
//         }
//       ssize_t n_read = read (fd, stash, BUFFER_SIZE);
//       if (n_read <= 0)
//         {
//           start = 0;
//           end = 0;
//           break;
//         }
//       start = 0;
//       end = n_read;
//       chunk_start = 0;
//       append_chunk (&chunks, stash, end - start);
//     }
//   size_t line_len = start;
//   char *line = concat_chunks (chunks, 0, line_len);
//   free_chunks (chunks);
//   return line;
// }

#include <fcntl.h>

// void
// test_concat ()
// {
//   t_chunk *c = NULL;
//   append_chunk (&c, "a123456789ABCDE ");
//   append_chunk (&c, "b123456789ABCDE ");
//   append_chunk (&c, "c123\n0000000000e");
//   char *s = concat_chunks (c, 0, 3);
//   int *a = malloc (4);
// }

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
