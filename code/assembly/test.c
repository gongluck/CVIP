// gcc -S test.c
// gcc -S test.c -m32 -o test_m32.s
// gcc -S test.c -m64 -o test_m64.s

// defalut type is cdecl
// m64没有差异？函数参数不使用堆栈传递，直接使用了寄存器了

// 函数调用结束后由被调用函数清除栈内数据
// #define CALLTYPE __attribute__((cdecl))
// 函数调用结束后由调用函数清除栈内数据
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
  asm volatile(
      "movl $0, %%eax\n\t"
      "addl %1, %%eax\n\t"
      "addl %2, %%eax\n\t"
      "movl %%eax, %0\n\t"
      : "=m"(val3)
      : "c"(val1), "d"(val2));
}

int main()
{
  return func(5);
}