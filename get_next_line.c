/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcronin <jcronin@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 05:13:01 by jcronin           #+#    #+#             */
/*   Updated: 2025/11/13 05:13:02 by jcronin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

t_chunk	*create_chunk(char *stash, size_t start, size_t len)
{
	t_chunk	*chunk;

	chunk = malloc(sizeof(*chunk));
	if (!chunk)
		return (0);
	chunk->data = malloc(BUFFER_SIZE);
	if (!chunk->data)
	{
		free(chunk);
		return (0);
	}
	chunk->len = len;
	ft_memcpy(chunk->data, &stash[start], len);
	chunk->next = NULL;
	return (chunk);
}

int	append_chunk(t_chunk **chunks, t_chunk *chunk)
{
	t_chunk	*tmp;

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

size_t	calc_chunks_len(t_chunk *chunks)
{
	size_t	count;

	count = 0;
	while (chunks)
	{
		count += chunks->len;
		chunks = chunks->next;
	}
	return (count);
}

// index in last chunk
char	*concat_chunks(t_chunk *chunks)
{
	size_t	len;
	char	*buf;
	size_t	i;

	if (!chunks)
		return (NULL);
	len = calc_chunks_len(chunks);
	buf = malloc(len);
	if (!buf)
		return (NULL);
	i = 0;
	while (chunks)
	{
		ft_memcpy(&buf[i], chunks->data, chunks->len);
		i += chunks->len;
		chunks = chunks->next;
	}
	buf[i] = '\0';
	return (buf);
}

t_chunk	*read_chunks(int fd, char *stash, size_t *start)
{
	t_chunk	*chunks;
	char	*newline;
	ssize_t	n_bytes;
	size_t	len;
	t_chunk	*chunk;

	chunks = NULL;
	newline = NULL;
	while (!newline)
	{
		if (*start == 0) // start at 0 means previous stash has been used
			n_bytes = read(fd, stash, BUFFER_SIZE);
		else // ehhhh static?
			n_bytes = BUFFER_SIZE;
		if (n_bytes == 0)
			break ;
		newline = ft_memchr(&(*stash)[start], '\n', n_bytes);
		len = n_bytes - *start; // ew but works?
		if (newline)
			len = newline - stash + 1;
		chunk = create_chunk(stash, *start, len);
		append_chunk(&chunks, chunk);
		*start = (newline && n_bytes == BUFFER_SIZE) * len; // what the fuck
	}
	return (chunks);
}

// if (newline)
// 	{
// 		size_t len = newline - stash + 1; // include newline
// 		// n_bytes >= len
// 		t_chunk *chunk = create_chunk (stash, last_start, len);
// 		start = len;
// 		append_chunk (&chunks, chunk);
// 		if (n_bytes < BUFFER_SIZE) start = 0;
// 		break ;
// 	}
// else
// 	{
// 		size_t len = n_bytes - last_start;
// 		t_chunk *chunk = create_chunk (stash, last_start, len);
// 		append_chunk (&chunks, chunk);
// 		start = 0;
// 	}

char	*get_next_line(int fd)
{
	static char		stash[BUFFER_SIZE];
	static size_t	start = 0;
	t_chunk			*chunks;
	char			*line;
	t_chunk			*tmp;

	chunks = read_chunks(fd, stash, &start);
	line = concat_chunks(chunks);
	while (chunks)
	{
		tmp = chunks->next;
		free(chunks->data);
		free(chunks);
		chunks = tmp;
	}
	return (line);
}

#include <fcntl.h>

int	main(void)
{
	int		fd;
	char	*l;

	fd = open("./testfile", O_RDONLY);
	while ((l = get_next_line(fd)))
	{
		printf("%s", l);
		free(l);
	}
	close(fd);
	return (0);
}
