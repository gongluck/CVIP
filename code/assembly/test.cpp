#include <iostream>

//defalut type is stdcall
//m64没有差异？函数参数不使用堆栈传递，直接使用了寄存器了

//函数调用结束后由被调用函数清除栈内数据
//#define CALLTYPE __attribute__((cdecl))
//函数调用结束后由调用函数清除栈内数据
#define CALLTYPE __attribute__((stdcall))

CALLTYPE int func(int v)
{
  return v * 2;
}

int main()
{
  return func(5);
}