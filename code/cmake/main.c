/*
 * @Author: gongluck 
 * @Date: 2020-05-17 14:42:02 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-05-17 17:46:14
 */

#include "config.h"
#include <stdio.h>

#ifdef USESUBMODULE
#include "myfun.h"
#endif//USESUBMODULE

int main()
{
    printf("hello, cmake!version:%d.%d\n", VERSION_MAJOR, VERSION_MINOR);

#ifdef USESUBMODULE
    printf("1+1=%d\n", myfun(1, 1));
#endif//USESUBMODULE

#ifdef HAVEPRINTF
    puts("found printf.");
#else
    puts("not found printf.");
#endif//HAVEPRINTF

    return 0;
}
