/*
 * @Author: gongluck
 * @Date: 2023-01-04 23:03:52
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-08 15:16:58
 */

// gcc -c SimpleSection.c -o SimpleSection.o

#include <stdio.h>

int globa1_init_var = 84;
int global_uninit_var;

void func1(int i)
{
  printf("%d\n", i);
}

int main(void)
{
  static int static_var = 85;
  static int static_var2;
  int a = 1;
  int b;
  func1(static_var + static_var2 + a + b);
  return 0;
}