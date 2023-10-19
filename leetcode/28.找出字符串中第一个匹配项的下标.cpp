/*
 * @lc app=leetcode.cn id=28 lang=cpp
 *
 * [28] 找出字符串中第一个匹配项的下标
 */

// @lc code=start
class Solution
{
public:
    int strStr(string haystack, string needle)
    {
        int N = haystack.size();
        int n = needle.size();
        int i = 0;
        for (; i <= N - n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (haystack[i + j] != needle[j])
                {
                    break;
                }
                else if (j == n - 1)
                {
                    return i;
                }
            }
        }

        return -1;
    }
};
// @lc code=end
