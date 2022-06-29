/*
 * @Author: gongluck
 * @Date: 2022-06-29 10:25:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2022-06-29 18:41:58
 */

#include <iostream>

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
    //GDB调试：
    // Program terminated with signal SIGSEGV, Segmentation fault.
    // #0  0x000055f44067b239 in main ()
    //查看main函数汇编代码：disassemble main
    // Dump of assembler code for function main:
    //    0x000055f44067b1c9 <+0>:     endbr64
    //    0x000055f44067b1cd <+4>:     push   %rbp
    //    0x000055f44067b1ce <+5>:     mov    %rsp,%rbp
    //    0x000055f44067b1d1 <+8>:     sub    $0x20,%rsp
    //    0x000055f44067b1d5 <+12>:    mov    %edi,-0x14(%rbp)
    //    0x000055f44067b1d8 <+15>:    mov    %rsi,-0x20(%rbp)
    //    0x000055f44067b1dc <+19>:    lea    0xe22(%rip),%rsi        # 0x55f44067c005
    //    0x000055f44067b1e3 <+26>:    lea    0x2e56(%rip),%rdi        # 0x55f44067e040 <_ZSt4cout@@GLIBCXX_3.4>
    //    0x000055f44067b1ea <+33>:    callq  0x55f44067b0a0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    //    0x000055f44067b1ef <+38>:    mov    %rax,%rdx
    //    0x000055f44067b1f2 <+41>:    mov    0x2dd7(%rip),%rax        # 0x55f44067dfd0
    //    0x000055f44067b1f9 <+48>:    mov    %rax,%rsi
    //    0x000055f44067b1fc <+51>:    mov    %rdx,%rdi
    //    0x000055f44067b1ff <+54>:    callq  0x55f44067b0b0 <_ZNSolsEPFRSoS_E@plt>
    //    0x000055f44067b204 <+59>:    movl   $0x0,-0xc(%rbp)
    //    0x000055f44067b20b <+66>:    cmpl   $0x1,-0x14(%rbp)
    //    0x000055f44067b20f <+70>:    jle    0x55f44067b227 <main+94>
    //    0x000055f44067b211 <+72>:    mov    -0x20(%rbp),%rax
    //    0x000055f44067b215 <+76>:    add    $0x8,%rax
    //    0x000055f44067b219 <+80>:    mov    (%rax),%rax
    //    0x000055f44067b21c <+83>:    mov    %rax,%rdi
    //    0x000055f44067b21f <+86>:    callq  0x55f44067b0d0 <atoi@plt>
    //    0x000055f44067b224 <+91>:    mov    %eax,-0xc(%rbp)
    //    0x000055f44067b227 <+94>:    cmpl   $0x0,-0xc(%rbp)
    //    0x000055f44067b22b <+98>:    jne    0x55f44067b241 <main+120>
    //    0x000055f44067b22d <+100>:   movq   $0x1111,-0x8(%rbp)
    //    0x000055f44067b235 <+108>:   mov    -0x8(%rbp),%rax
    // => 0x000055f44067b239 <+112>:   movl   $0x1,(%rax)
    //    0x000055f44067b23f <+118>:   jmp    0x55f44067b242 <main+121>
    //    0x000055f44067b241 <+120>:   nop
    //    0x000055f44067b242 <+121>:   mov    $0x0,%eax
    //    0x000055f44067b247 <+126>:   leaveq
    //    0x000055f44067b248 <+127>:   retq
    // End of assembler dump.
    //剩下就是看汇编结合源码分析了
    //或者启动gdb时指定调试符号--symbol=
    *pcrash = 1;
  }
  break;
  default:
    break;
  }

  return 0;
}