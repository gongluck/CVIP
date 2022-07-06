/*
 * @Author: gongluck
 * @Date: 2022-06-29 19:00:22
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-07-01 14:13:20
 */

#include "submodule_shared.h"

int crash_shared(int a, int b)
{
  int *pcrash = nullptr;

  pcrash = (int *)a;
  *pcrash = b;

  return 0;
}