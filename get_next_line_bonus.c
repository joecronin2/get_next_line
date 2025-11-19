/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      :::::::: */
/*   get_next_line_bonus.c                              :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+ */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+ */
/*   Created: 2025/11/17 21:10:34 by jcronin           #+#    #+# */
/*   Updated: 2025/11/17 21:14:07 by jcronin          ###   ########.fr */
/*                                                                            */
/* **************************************************************************
 */

#include "get_next_line.h"

t_chunk *
create_chunk (unsigned char *str, size_t len)
{
  t_chunk *chunk;

  chunk = malloc (sizeof (*chunk));
  if (!chunk) return (NULL);
  chunk->data = malloc (BUFFER_SIZE);
  if (!chunk->data)
    {
      free (chunk);
      return (NULL);
    }
  chunk->len = len;
  ft_memcpy (chunk->data, str, len);
  chunk->next = NULL;
  return (chunk);
}

int
append_chunk (t_chunk **chunks, t_chunk *chunk)
{
  t_chunk *tmp;

  if (!*chunks)
    *chunks = chunk;
  else
    {
      tmp = *chunks;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = chunk;
    }
  return (1);
}

size_t
calc_chunks_len (t_chunk *chunks)
{
  size_t count;

  count = 0;
  while (chunks)
    {
      count += chunks->len;
      chunks = chunks->next;
    }
  return (count);
}

void
free_chunks (t_chunk *chunks)
{
  t_chunk *tmp;

  while (chunks)
    {
      tmp = chunks->next;
      free (chunks->data);
      free (chunks);
      chunks = tmp;
    }
}
