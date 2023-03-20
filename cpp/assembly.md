# 汇编

- [汇编](#汇编)
  - [寄存器](#寄存器)
  - [函数调用](#函数调用)

## 寄存器

- `ESP`：堆栈指针(stack pointer)。
- `EBP`：基址指针(base pointer)，在 C 语言中用作记录当前函数调用基址。
- `CS`：代码段寄存器。
- `EIP`：总是指向下一条的指令地址。
  - `call`：将当前`CS:EIP`的值压入栈顶，CS:EIP 指向被调用函数的入口地址。
  - `ret`：从栈顶弹出原来保存在这里的 CS:EIP 的值，放入 CS:EIP 中。

## 函数调用

[函数调用](../code/assembly)

<details>
<summary>C++</summary>

```c++
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
```
</details>

<details>
<summary>Assembly</summary>

```asm
main:
.LFB1520:
  .cfi_startproc
  endbr32
  pushl	%ebp                          #旧基址进栈保存
  .cfi_def_cfa_offset 8
  .cfi_offset 5, -8
  movl	%esp, %ebp                    #新栈(基址)
  .cfi_def_cfa_register 5
  call	__x86.get_pc_thunk.ax         #把esp的内容赋值给eax (movl (%esp),%eax)
  addl	$_GLOBAL_OFFSET_TABLE_, %eax
  pushl	$5                            #函数参数入栈
  call	_Z4funci                      #函数调用
  addl	$4, %esp                      #函数参数出栈
  nop
  leave
  .cfi_restore 5
  .cfi_def_cfa 4, 4
  ret                                 #函数返回
  .cfi_endproc
```
</details>
