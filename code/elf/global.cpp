/*
 * @Author: gongluck
 * @Date: 2022-04-14 10:49:56
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-04-14 10:53:06
 */

// application rewrite it`s global variable via shell tools
#include <stdio.h>
#include <stdlib.h>
#define NAME2STR(name) (#name)
int GGG = 1;
int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    int n = atoi(argv[2]);
    FILE *fp = fopen(argv[0], "r+b");
    fseek(fp, atoi(argv[1]), SEEK_SET);
    fwrite(&n, 4, 1, fp);
    fclose(fp);
  }
  else
  {
    printf("%s\n", argv[0]);
    printf("%d\n", GGG);
    srand(GGG);
    GGG = rand();
    char buf[1024] = {0};
    sprintf(buf, "%s $(expr `printf %%d 0x$(readelf -s %s | grep %s | awk '{print $2}')` - `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $4}')` + `printf %%d 0x$(readelf -S %s | grep \" .data \" | awk '{print $5}')`) %d",
            argv[0], argv[0], NAME2STR(GGG), argv[0], argv[0], GGG);
    system(buf);
  }
  return 0;
}