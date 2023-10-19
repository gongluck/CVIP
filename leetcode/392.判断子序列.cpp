/*
 * @lc app=leetcode.cn id=392 lang=cpp
 *
 * [392] 判断子序列
 */

// @lc code=start
class Solution
{
public:
    bool isSubsequence(string s, string t)
    {
        int n = t.size();
        int pos = 0;
        int matched = 0;

        for (const auto &c : s)
        {
            while (pos < n)
            {
                if (t[pos++] == c)
                {
                    ++matched;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        return matched == s.size();
    }
};
// @lc code=end
