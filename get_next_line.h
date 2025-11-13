/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 03:09:32 by jcronin           #+#    #+#             */
/*   Updated: 2025/11/13 03:09:33 by jcronin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdlib.h>

#define BUFFER_SIZE 16

typedef struct s_chunk
{
	char			*data;
	size_t			len;
	struct s_chunk	*next;
}					t_chunk;

typedef struct s_stash
{
	unsigned char	stash[BUFFER_SIZE];
	size_t			len;
	size_t			start;
}					t_stash;

char				*get_next_line(int fd);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
ssize_t				newline_index(const void *s, size_t n);
