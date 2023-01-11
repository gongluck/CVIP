/*
 * @Author: gongluck
 * @Date: 2023-01-11 16:21:55
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-11 16:24:25
 */

// gcc -fpic -shared -o Lib.so Lib.c

#include <stdio.h>

void foobar(int i)
{
  printf("Printing from Lib.so: %d\n", i);
}
