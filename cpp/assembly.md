# 汇编

- [汇编](#汇编)
  - [函数调用](#函数调用)

## 函数调用

  [函数调用](../code/assembly)

  <details>
  <summary>C++</summary>

  ```C++
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
  ```
  </details>

  <details>
  <summary>Assembly</summary>

  ```Assembly
  main:
  .LFB1520:
    .cfi_startproc
    endbr32
    pushl	%ebp 													#旧基址进栈保存
    .cfi_def_cfa_offset 8
    .cfi_offset 5, -8
    movl	%esp, %ebp										#新栈(基址)
    .cfi_def_cfa_register 5
    call	__x86.get_pc_thunk.ax					#把esp的内容赋值给eax (movl (%esp),%eax)
    addl	$_GLOBAL_OFFSET_TABLE_, %eax
    pushl	$5														#函数参数入栈
    call	_Z4funci											#函数调用
    addl	$4, %esp											#函数参数出栈
    nop
    leave
    .cfi_restore 5
    .cfi_def_cfa 4, 4
    ret																	#函数返回
    .cfi_endproc
  ```
  </details>
