/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      :::::::: */
/*   get_next_line.h                                    :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+ */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+ */
/*   Created: 2025/11/13 03:09:32 by jcronin           #+#    #+# */
/*   Updated: 2025/11/17 21:12:57 by jcronin          ###   ########.fr */
/*                                                                            */
/* **************************************************************************
 */

#pragma once

#include <stdlib.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 8
#endif

typedef struct s_stash
{
  unsigned char stash[BUFFER_SIZE];
  size_t len;
  size_t start;
	// int found_newline;
} t_stash;



char *get_next_line (int fd);
void *ft_memchr (const void *s, int c, size_t n);
void *ft_memcpy (void *dest, const void *src, size_t n);
ssize_t newline_index (const void *s, size_t n);

#include "get_next_line_bonus.h"
t_chunk *get_chunk (t_stash *stash);
