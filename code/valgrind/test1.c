/*
 * @Author: gongluck 
 * @Date: 2020-12-03 14:47:31 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-03 15:09:53
 */

// gcc -g -O0 test1.c

#include <stdio.h>
#include <stdlib.h>

void fun()
{
    int *p = (int *)malloc(10 * sizeof(int));
    p[10] = 0;
}

int main(void)
{
    fun();
    return 0;
}

/*
$ valgrind ./a.out
==12740== Memcheck, a memory error detector
==12740== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==12740== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==12740== Command: ./a.out
==12740== 
==12740== Invalid write of size 4
==12740==    at 0x108668: fun (test1.c:16)
==12740==    by 0x10867E: main (test1.c:21)
==12740==  Address 0x5230068 is 0 bytes after a block of size 40 alloc'd
==12740==    at 0x4C31ECB: malloc (vg_replace_malloc.c:307)
==12740==    by 0x10865B: fun (test1.c:15)
==12740==    by 0x10867E: main (test1.c:21)
==12740== 
==12740== 
==12740== HEAP SUMMARY:
==12740==     in use at exit: 40 bytes in 1 blocks
==12740==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==12740== 
==12740== LEAK SUMMARY:
==12740==    definitely lost: 40 bytes in 1 blocks
==12740==    indirectly lost: 0 bytes in 0 blocks
==12740==      possibly lost: 0 bytes in 0 blocks
==12740==    still reachable: 0 bytes in 0 blocks
==12740==         suppressed: 0 bytes in 0 blocks
==12740== Rerun with --leak-check=full to see details of leaked memory
==12740== 
==12740== For lists of detected and suppressed errors, rerun with: -s
==12740== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/