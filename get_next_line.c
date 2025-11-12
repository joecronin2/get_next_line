#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

char *
realloc_buffer (char *buf, size_t *oldsize)
{
  if (!*oldsize)
    {
      *oldsize = BUFFER_SIZE;
      return malloc (*oldsize);
    }
  if ((*oldsize + BUFFER_SIZE) <= (*oldsize * 2)) return buf;
  char *new = malloc (*oldsize *= 2);
  if (!new) return NULL;
  ft_memcpy (new, buf, *oldsize);
  free (buf);
  return new;
}

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

char *
get_next_line (int fd)
{
  // static char *stash[FD_SETSIZE];
  // static size_t fptr[FD_SETSIZE];
	char *buf;
  static char stash[BUFFER_SIZE];
  static size_t index = 0;
  size_t start = index;
  void *found = NULL;
  static size_t curr_size = 0;
  while (!found)
    {
      stash = realloc_buffer (stash, &curr_size);
      if (!stash) return NULL;
      ssize_t n_read = read (fd, stash, BUFFER_SIZE);
      if (n_read == -1)
        {
          free (stash);
          return NULL;
        }
      found = ft_memchr (&stash[index], '\n', curr_size);
      index = (char *)found - (char *)stash;
    }
  char *res = ft_substr (stash, start, index + 1);
  free (stash);
  return res;
}

#include <fcntl.h>

int
main ()
{
  int fd = open ("./testfile", O_RDONLY);
  char *l = get_next_line (fd);
  l = get_next_line (fd);
  return 0;
}
