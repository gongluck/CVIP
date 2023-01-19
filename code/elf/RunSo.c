/*
 * @Author: gongluck
 * @Date: 2023-01-19 10:09:40
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-19 10:10:55
 */

#include <stdio.h>
#include <dlfcn.h>
#include <setjmp.h>

jmp_buf buf;

void error_code(void)
{
  longjmp(buf, 1);
}

typedef void (*func)();
typedef void (*func1)(void *s);
typedef void (*func2)(void *s, void *c);
typedef void (*func3)(void *s, void *c, void *m);
int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("runso file functionname\n");
    return -1;
  }
  static void *handle = NULL;
  char *file_name = argv[1];
  char *function = argv[2];
  handle = dlopen(file_name, RTLD_LAZY);
  if (!handle)
  {
    printf("dlopen error!\n");
    return 0;
  }
  func ptr = dlsym(handle, function);
  if (!ptr)
  {
    printf("Can't find this function!\n");
    return 0;
  }
  printf("%s address: %p\n", function, ptr);
  if (setjmp(buf) == 0)
  {
    ptr("1");
  }
  else
  {
    if (setjmp(buf) == 0)
    {
      ptr("2", "3");
    }
    else
    {
      if (setjmp(buf) == 0)
      {
        ptr();
      }
      else
      {
        ptr("4", "5", "6");
      }
    }
  }
  return 0;
}
