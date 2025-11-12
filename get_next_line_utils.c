#include <stddef.h>
#include <stdlib.h>

void *
ft_memchr (const void *s, int c, size_t n)
{
  size_t i;

  i = 0;
  while (i < n)
    {
      if (((unsigned char *)s)[i] == c) return (&((unsigned char *)s)[i]);
      i++;
    }
  return (0);
}

void *
ft_memcpy (void *dest, const void *src, size_t n)
{
  size_t i;

  i = 0;
  while (i < n)
    {
      ((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
      i++;
    }
  return (dest);
}

size_t
ft_strlen (const char *s)
{
  const char *p = s;

  while (*p)
    p++;
  return (p - s);
}

char *
ft_substr (char const *s, unsigned int start, size_t len)
{
  char *b;
  size_t maxlen;

  maxlen = ft_strlen (s);
  if (start >= maxlen) return (ft_substr ("", 0, 1));
  if (start + len > maxlen) len = maxlen - start;
  b = malloc (len + 1);
  if (!b) return (NULL);
  ft_memcpy (b, s + start, len);
  b[len] = '\0';
  return (b);
}
