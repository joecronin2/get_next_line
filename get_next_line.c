/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      :::::::: */
/*   get_next_line.c                                    :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+ */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+ */
/*   Created: 2025/11/13 05:13:01 by jcronin           #+#    #+# */
/*   Updated: 2025/11/13 05:13:02 by jcronin          ###   ########.fr */
/*                                                                            */
/* **************************************************************************
 */

#include "get_next_line.h"
#include "get_next_line_bonus.h"
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>

char *
concat_chunks (t_chunk *chunks)
{
  size_t len;
  char *buf;
  size_t i;

  if (!chunks) return (NULL);
  len = calc_chunks_len (chunks);
  buf = malloc (len + 1);
  if (!buf) return (NULL);
  i = 0;
  while (chunks)
    {
      ft_memcpy (&buf[i], chunks->data, chunks->len);
      i += chunks->len;
      chunks = chunks->next;
    }
  buf[i] = '\0';
  return (buf);
}

size_t
chunk_len (const unsigned char *s, size_t n)
{
  size_t i;

  i = 0;
  while (i < n)
    {
      if (s[i] == '\n') return (i + 1);
      i++;
    }
  return (i);
}

t_chunk *
get_chunk (t_stash *stash)
{
  size_t max;
  size_t len;
  t_chunk *chunk;

  max = stash->len - stash->start;
  if (max == 0) return (NULL);
  len = chunk_len (&stash->stash[stash->start], max);
  chunk = create_chunk (&stash->stash[stash->start], len);
  // if (len < stash->len)
  stash->start = stash->start + len;
  // else
  //   stash->start = 0;
  return (chunk);
}

t_chunk *
read_chunks (int fd, t_stash *stash)
{
  t_chunk *chunks;
  t_chunk *chunk;

  chunks = NULL;
  while (1)
    {
      if (stash->start == 0)
        {
          stash->len = read (fd, stash, BUFFER_SIZE);
          if (stash->len == 0) break;
        }
      chunk = get_chunk (stash);
      if (stash->start >= stash->len) stash->start = 0;
      if (!chunk) break;
      append_chunk (&chunks, chunk);
      if (chunk->data[chunk->len - 1] == '\n') break;
    }
  return (chunks);
}

char *
get_next_line (int fd)
{
  static t_stash stash;
  t_chunk *chunks;
  char *line;

  chunks = read_chunks (fd, &stash);
  line = concat_chunks (chunks);
  free_chunks (chunks);
  return line;
}
