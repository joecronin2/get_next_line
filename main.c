#include "get_next_line.h"
#include "get_next_line_bonus.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void
test_get_chunk ()
{
  t_stash s = { { '1', '2', '3', '4', '5', '6', '7', '8' }, 8, 0 };
  t_chunk *c = get_chunk (&s);
  assert (c->len == 8);
  assert (memcmp (s.stash, c->data, c->len) == 0);
  assert (s.start == 8);

  t_stash s1 = { { '1', '2', '3', '4', '5', '6', '7', '8' }, 8, 3 };
  c = get_chunk (&s1);
  assert (c->len == 5);
  assert (memcmp ("45678", c->data, c->len) == 0);
  assert (s1.start == 8);

  t_stash s2 = { { '1', '2', '3', '4', '5', '6', '7', '\n' }, 8, 3 };
  c = get_chunk (&s2);
  assert (c->len == 5);
  assert (memcmp ("4567\n", c->data, c->len) == 0);
  assert (s2.start == 8);

  t_stash s3 = { { '1', '2', '3', '4', '5', '6', '\n', '8' }, 8, 3 };
  c = get_chunk (&s3);
  assert (c->len == 4);
  assert (memcmp ("456\n", c->data, c->len) == 0);
  assert (s3.start == 7);
}

int
main (int argc, char *argv[])
{
  // test_get_chunk ();
  int fd;
  char *l;
  int i;
  if (argc == 0)
    {
      fd = open ("./testfile", O_RDONLY);
      i = 0;
      while ((l = get_next_line (fd)))
        {
          printf ("%d: %s", i++, l);
          free (l);
        }
      printf ("lines read: %d\n", i);
      close (fd);
    }
  else
    {
      fd = open (argv[1], O_RDONLY);
      i = 0;
      while ((l = get_next_line (fd)))
        {
          printf ("%d: %s", i++, l);
          free (l);
        }
      printf ("lines read: %d\n", i);
      close (fd);
    }

  return (0);
}
