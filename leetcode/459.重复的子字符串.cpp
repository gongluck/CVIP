/*
 * @lc app=leetcode.cn id=459 lang=cpp
 *
 * [459] 重复的子字符串
 */

// @lc code=start
class Solution
{
public:
    bool repeatedSubstringPattern(string s)
    {
        int n = s.size();
        std::vector<int> next(n, 0);

        for (int i = 1; i < n; ++i)
        {
            int j = next[i - 1];
            while (j > 0 && s[i] != s[j])
            {
                j = next[j - 1];
            }
            if (s[i] == s[j])
            {
                next[i] = ++j;
            }
        }

        return next[n - 1] != 0 && n % (n - next[n - 1]) == 0;
    }
};
// @lc code=end
