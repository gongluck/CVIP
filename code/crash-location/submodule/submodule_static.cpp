/*
 * @Author: gongluck
 * @Date: 2022-06-29 19:00:22
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-07-01 14:10:24
 */

#include "submodule_static.h"

int crash_static(int a, int b)
{
  int *pcrash = nullptr;

  pcrash = (int *)a;
  *pcrash = b;

  return 0;
}