/*
 * @Author: gongluck 
 * @Date: 2020-12-03 15:25:54 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-03 18:21:22
 */


// gcc -g -O0 test1.c

#include <stdio.h>

int main(void)
{
    printf("hello world! dump core for set value to NULL pointer/n");
    *(char *)0 = 0;
    return 0;
}
