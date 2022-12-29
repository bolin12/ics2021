#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s != '\0') {
    len++;
    s++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *ret = dst;
  while (*src != '\0') {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *ret = dst;
  while (*src != '\0' && n > 0) {
    *dst = *src;
    dst++;
    src++;
    n--;
  }
  while (n > 0) {
    *dst = '\0';
    dst++;
    n--;
  }
  return ret;
}

char *strcat(char *dst, const char *src) {
  char *ret = dst;
  while (*dst != '\0') {
    dst++;
  }
  while (*src != '\0') {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ret;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 != '\0' && *s2 != '\0') {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (*s1 != '\0' && *s2 != '\0' && n > 0) {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
    n--;
  }
  if (n == 0) {
    return 0;
  }
  return *s1 - *s2;
}

void *memset(void *s, int c, size_t n) {
  uint8_t *p = s;
  while (n > 0) {
    *p = c;
    p++;
    n--;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  uint8_t *d = dst;
  const uint8_t *s = src;
  if (d < s) {
    while (n > 0) {
      *d = *s;
      d++;
      s++;
      n--;
    }
  } else {
    d += n;
    s += n;
    while (n > 0) {
      d--;
      s--;
      *d = *s;
      n--;
    }
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  uint8_t *d = out;
  const uint8_t *s = in;
  while (n > 0) {
    *d = *s;
    d++;
    s++;
    n--;
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = s1;
  const uint8_t *p2 = s2;
  while (n > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
    n--;
  }
  return 0;
}

#endif
