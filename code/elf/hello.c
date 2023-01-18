/*
 * @Author: gongluck
 * @Date: 2023-01-03 22:30:53
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-12 16:15:16
 */

// gcc -E hello.c -o hello.i
// gcc -S hello.c -o hello.s
// gcc -c hello.c -o hello.o

#include <stdio.h>

int main()
{
  printf("Hello World\n");
  sleep(-1);
  return 0;
}