
#include "bloomfilter.h"
#include <stdio.h>

#define MAX_ITEMS 6000000 // 设置最大元素个数
#define ADD_ITEMS 1000    // 添加测试元素
#define P_ERROR 0.0001    // 设置误差

int main(int argc, char **argv)
{

    printf("test bloomfilter\n");

    // 1. 定义BaseBloomFilter
    static BaseBloomFilter stBloomFilter = {0};

    // 2. 初始化stBloomFilter，调用时传入hash种子，存储容量，以及允许的误判率
    InitBloomFilter(&stBloomFilter, 0, MAX_ITEMS, P_ERROR);

    // 3. 向BloomFilter中新增数值
    char url[128] = {0};
    for (int i = 0; i < ADD_ITEMS; i++)
    {
        sprintf(url, "https://test.com/%d.html", i);
        if (0 == BloomFilter_Add(&stBloomFilter, (const void *)url, strlen(url)))
        {
            // printf("add %s success", url);
        }
        else
        {
            printf("add %s failed", url);
        }
        memset(url, 0, sizeof(url));
    }

    // 4. check url exist or not
    char *str = "https://test.com/0.html";
    if (0 == BloomFilter_Check(&stBloomFilter, (const void *)str, strlen(str)))
    {
        printf("https://test.com/0.html exist\n");
    }

    char *str2 = "https://test.com/10001.html";
    if (0 != BloomFilter_Check(&stBloomFilter, (const void *)str2, strlen(str2)))
    {
        printf("https://test.com/10001.html not exist\n");
    }

    // 5. free bloomfilter
    FreeBloomFilter(&stBloomFilter);
    getchar();
    return 0;
}
