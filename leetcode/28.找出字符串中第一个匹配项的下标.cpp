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
        std::vector<int> next(n, 0);

        for (int i = 1; i < n - 1; ++i)
        {
            int j = next[i - 1];
            while (j > 0 && needle[i] != needle[j])
            {
                j = next[j - 1];
            }
            if (needle[i] == needle[j])
            {
                next[i] = ++j;
            }
        }

        for (int i = 0, j = 0; i < N; ++i)
        {
            while (j > 0 && haystack[i] != needle[j])
            {
                j = next[j - 1];
            }
            if (haystack[i] == needle[j])
            {
                if (++j == n)
                {
                    return i - n + 1;
                }
            }
        }

        return -1;
    }
};
// @lc code=end
