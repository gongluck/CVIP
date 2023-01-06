/*
 * @Author: gongluck
 * @Date: 2023-01-06 17:24:09
 * @Last Modified by:   gongluck
 * @Last Modified time: 2023-01-06 17:24:09
 */

#include <stdio.h>

extern char __executable_start[];
extern char etext[], _etext[], __etext[];
extern char edata[], _edata[];
extern char end[], _end[];

int main()
{
  printf("Executable Start %X\n", __executable_start);
  printf("Text End %X %X %X\n", etext, _etext, __etext);
  printf("Data End %X %X\n", edata, _edata);
  printf("Executable End %X %X\n", end, _end);
  return 0;
}