/*
 * @Author: gongluck 
 * @Date: 2020-11-18 07:52:34 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-18 09:28:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>

int main()
{
    iconv_t cd = iconv_open("GBK", "UTF-8"); //UTF8 -> GBK
    if (cd == (iconv_t)-1)
    {
        printf("iconv_open failed.\n");
        exit(-1);
    }

    char str[] = "iconv例子";
    char *pstr = str;
    size_t len = sizeof(str);
    char out[100] = {0};
    char *pout = out;
    size_t leftlen = sizeof(out);
    while (1)
    {
        pout = out;
        leftlen = sizeof(out);
        int n = iconv(cd, &pstr, &len, &pout, &leftlen);
        //printf("iconv return %d\n", n);
        if (pout != out)//有转换成功字符
        {
            printf("%.*s", (int)(sizeof(out)-leftlen), out);
        }
        else
        {
            break;
        }
        
        if(n != -1)
        {
            pout = out;
			leftlen = sizeof(out);
			n = iconv(cd, NULL, NULL, &pout, &leftlen);
			if (pout != out) // 检测iconv内部是否还有缓存
			{
				printf("%.*s", (int)(sizeof(out)-leftlen), out);
			}
			break;
        }
    }
    printf("\n");
    iconv_close(cd);

    return 0;
}