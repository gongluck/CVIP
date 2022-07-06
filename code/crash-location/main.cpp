/*
 * @Author: gongluck
 * @Date: 2022-06-29 10:25:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-07-01 15:13:58
 */

#include <iostream>
#include "submodule/submodule_static.h"
#include "submodule/submodule_shared.h"

// gdb debugexe coredump
// set solib-search-path 动态库路径
int main(int argc, char *argv[])
{
  std::cout << "test crash-location" << std::endl;

  // type-0:crash in main
  int type = 0;
  if (argc > 1)
  {
    type = atoi(argv[1]);
  }

  switch (type)
  {
  case 0:
  {
    int *pcrash = (int *)0x1111;
    *pcrash = 1;
  }
  break;
  case 1:
  {
    crash_static(1, 2);
  }
  break;
  case 2:
  {
    crash_shared(3, 4);
  }
  break;
  default:
    break;
  }

  return 0;
}