/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 22:44:01 by jcronin           #+#    #+#             */
/*   Updated: 2025/11/19 22:44:01 by jcronin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void append_chunk (t_chunk **chunks, t_chunk *chunk);
void free_chunks (t_chunk *chunks);
