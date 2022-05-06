#include <iostream>

// defalut type is cdecl
// m64没有差异？函数参数不使用堆栈传递，直接使用了寄存器了

//函数调用结束后由被调用函数清除栈内数据
//#define CALLTYPE __attribute__((cdecl))
//函数调用结束后由调用函数清除栈内数据
#define CALLTYPE __attribute__((stdcall))

CALLTYPE int func(int v)
{
  return v * 2;
}

void func_asm()
{
  unsigned int val1 = 1;
  unsigned int val2 = 2;
  unsigned int val3 = 0;

  // std::cout << "val1:" << val1 << ", val2:" << val2 << ", val3:" << val3 << std::endl;
  // val3 = val1 + val2;
  asm volatile(
      "movl $0, %%eax\n\t"
      "addl %1, %%eax\n\t"
      "addl %2, %%eax\n\t"
      "movl %%eax, %0\n\t"
      : "=m"(val3)
      : "c"(val1), "d"(val2));
  // std::cout << "val1:" << val1 << ", val2:" << val2 << ", val3:" << val3 << std::endl;
}

int main()
{
  return func(5);
}