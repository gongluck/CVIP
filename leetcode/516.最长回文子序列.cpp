/*
 * @lc app=leetcode.cn id=516 lang=cpp
 *
 * [516] 最长回文子序列
 */

// @lc code=start
class Solution
{
public:
    int longestPalindromeSubseq(string s)
    {
        std::vector<std::vector<int>> dp(s.size(), std::vector<int>(s.size(), 0));

        for (int i = 0; i < s.size(); ++i)
        {
            dp[i][i] = 1;
        }

        for (int left = s.size() - 2; left >= 0; --left)
        {
            for (int right = left + 1; right < s.size(); ++right)
            {
                if (s[left] == s[right])
                {
                    dp[left][right] = dp[left + 1][right - 1] + 2;
                }
                else
                {
                    dp[left][right] = std::max(dp[left + 1][right], dp[left][right - 1]);
                }
            }
        }

        return dp[0][s.size() - 1];
    }
};
// @lc code=end
