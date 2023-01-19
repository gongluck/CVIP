/*
 * @Author: gongluck
 * @Date: 2023-01-19 09:55:52
 * @Last Modified by:   gongluck
 * @Last Modified time: 2023-01-19 09:55:52
 */

#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
  void *handle;
  double (*func)(double);
  char *error;

  if ((handle = dlopen(argv[1], RTLD_NOW)) == NULL)
  {
    printf("Open library %s error: %s\n",
           argv[1], dlerror());
    return -1;
  }
  func = dlsym(handle, "sin");
  if ((error = dlerror()) != NULL)
  {
    printf("Symbol sin not found: %s\n", error);
  }
  else
  {
    printf("%f\n", func(3.1415926 / 2));
  }
  dlclose(handle);
}