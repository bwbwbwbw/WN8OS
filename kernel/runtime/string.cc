/**
 * 基本字符串处理，来自 glibc / Apple libc
 */

#include <runtime/string.h>

extern "C" {

/* Append SRC on the end of DEST.  */
char *
strcat (char *dest, const char *src)
{
  strcpy (dest + strlen (dest), src);
  return dest;
}

char *
strchr(const char *s, int c)
{
    const char ch = c;

    for ( ; *s != ch; s++)
        if (*s == '\0')
            return NULL;
    return (char *)s;
}

int
strcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
  return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}

char *
strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while ((*s++ = *s2++) != 0)
  ;
    return (s1);
}

size_t
strlen(const char *str)
{
  register const char *s;

  for (s = str; *s; ++s);
  return(s - str);
}


}