/*
 * @Author: gongluck
 * @Date: 2023-01-18 00:27:38
 * @Last Modified by:   gongluck
 * @Last Modified time: 2023-01-18 00:27:38
 */

#include <stdio.h>
#include <elf.h>

int main(int argc, char *argv[])
{
  int *p = (int *)argv;
  int i;
  Elf32_auxv_t *aux;

  printf("Argument count: %d\n", *(p - 1));

  for (i = 0; i < *(p - 1); i++)
  {
    printf("Argument %d: %s\n", i, *(p + 1));
  }

  p += i;
  p++; // skip 0

  printf("Environment: \n");
  while (*p)
  {
    printf("%s\n", *p);
    p++;
  }

  p++; // skip 0

  printf("Auxiliary Vectors: \n");
  aux = (Elf32_auxv_t *)p;
  while (aux->a_type != AT_NULL)
  {
    printf("Type: %02d Value: %x\n", aux->a_type, aux->a_un.a_val);
    aux++;
  }

  return 0;
}
