/*
 * @Author: gongluck 
 * @Date: 2020-11-05 18:25:51 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-05 21:39:06
 */

#include <stdio.h>
#include <string.h>

void make_next(const char *pattern, int *next)
{
    int q, k;// k前缀下标，q后缀下标
    int m = strlen(pattern);

    next[0] = 0;
    for (q = 1, k = 0; q < m; q++)
    {
        while (k > 0 && pattern[q] != pattern[k])
            k = next[k - 1];

        if (pattern[q] == pattern[k])
        {
            k++;
        }

        next[q] = k;
    }

    // abcabc
    // next[0] = 0;
    // q=1, k=0, pattern[q]:pattern[k] = b:a, next[1] = 0;
    // q=2, k=0, pattern[q]:pattern[k] = c:a, next[2] = 0;
    // q=3, k=0, pattern[q]:pattern[k] = a:a, k++, next[3] = 1;
    // q=4, k=1, pattern[q]:pattern[k] = b:b, k++, next[4] = 2;
    // q=5, k=2, pattern[q]:pattern[k] = c:c, k++, next[5] = 3;
    // q=6, k=3, pattern[q]:pattern[k] = d:a, k=next[k-1] -> k=0; next[6] = 0;
}

int kmp(const char *text, const char *pattern, int *next)
{
    int n = strlen(text);
    int m = strlen(pattern);
    make_next(pattern, next);

    int i, q;
    for (i = 0, q = 0; i < n; i++)
    {
        while (q > 0 && pattern[q] != text[i])
        {
            q = next[q - 1];
        }

        if (pattern[q] == text[i])
        {
            q++;
        }

        if (q == m)
        {
            break;
        }
    }

    return i - q + 1;
}

int main()
{
    int i;
    int next[20] = {0};

    char *text = "ababxbababababcdababcabddcadfdsss";
    char *pattern = "abcabd";

    int idx = kmp(text, pattern, next);
    printf("match pattern : %d\n", idx);

    for (i = 0; i < strlen(pattern); i++)
    {
        printf("%4d", next[i]);
    }
    printf("\n");

    return 0;
}