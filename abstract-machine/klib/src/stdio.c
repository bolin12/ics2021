#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


char *_strrev (char *str)
{
  int i;
  int len = 0;
  char c;
  if (!str)
    return NULL;
  while(str[len] != '\0'){
    len++;
  }
  for(i = 0; i < (len/2); i++)
  {
    c = str[i];
    str [i] = str[len - i - 1];
    str[len - i - 1] = c;
  }
  return str;
}

char * _itoa(int i, char *strout, int base)
{
  char *str = strout;
  int digit, sign = 0;
  if (i < 0) {
    sign = 1;
    i *= -1;
  }
  while(i) {
    digit = i % base;
    *str = (digit > 9) ? ('A' + digit - 10) : '0' + digit;
    i = i / base;
    str ++;
  }
  if(sign) {
  *str++ = '-';
  }
  *str = '\0';
  _strrev(strout);
  return strout;
} 

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  // panic("Not implemented");
  va_list vl;
  int i=0,j=0;
  char buff[100]={0},tmp[20];
  char *str_arg;
  va_start(vl, fmt);
  while(fmt[i]){
    if(fmt[i]=='%'){
      i++;
      switch (fmt[i])
      {
      case 's':
        str_arg=va_arg(vl, char*);
        strcpy(&buff[j], str_arg);
        j+=strlen(str_arg);
        break;
      case 'd':
        _itoa(va_arg( vl, int ), tmp, 10);
        strcpy(&buff[j], tmp);
        j += strlen(tmp);
        break; 
      default:
        break;
      }
    }
    else{
      buff[j] = fmt[i];
      j++;
    }
    i++;
  }
  strcpy(out, buff);
  va_end(vl);
  return j;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
