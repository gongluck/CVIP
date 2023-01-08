/*
 * @Author: gongluck
 * @Date: 2023-01-08 14:48:13
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-01-08 15:00:21
 */

// gcc -c -fno-builtin -m32 TinyHelloWorld.c
// ld -static -m elf_i386 -e nomain -o TinyHelloWorld TinyHelloWorld.o
// ld -static -m elf_i386 -s -T TinyHelloWorld.lds -o TinyHelloWorld TinyHelloWorld.o
char *str = "Hello World!\n";

void print()
{
  asm("movl $13,%%edx \n\t"
      "movl  %0,%%ecx \n\t"
      "movl $0,%%ebx \n\t"
      "movl $4,%%eax \n\t"
      "int $0x80     \n\t" ::"r"(str)
      : "edx", "ecx", "ebx");
}

void exit()
{
  asm("movl $42,%ebx \n\t"
      "movl $1,%eax \n\t"
      "int $0x80    \n\t");
}

void nomain()
{
  print();
  exit();
}