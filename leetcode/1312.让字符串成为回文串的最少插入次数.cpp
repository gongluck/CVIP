/*
 * @lc app=leetcode.cn id=1312 lang=cpp
 *
 * [1312] 让字符串成为回文串的最少插入次数
 */

// @lc code=start
class Solution
{
public:
    int minInsertions(string s)
    {
        std::vector<std::vector<int>> dp(s.size(), std::vector<int>(s.size(), 0));

        for (int left = s.size() - 2; left >= 0; --left)
        {
            for (int right = left + 1; right < s.size(); ++right)
            {
                if (s[left] == s[right])
                {
                    dp[left][right] = dp[left + 1][right - 1];
                }
                else
                {
                    dp[left][right] = std::min(dp[left][right - 1], dp[left + 1][right]) + 1;
                }
            }
        }

        return dp[0][s.size() - 1];
    }
};
// @lc code=end
