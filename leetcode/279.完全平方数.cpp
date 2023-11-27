/*
 * @lc app=leetcode.cn id=279 lang=cpp
 *
 * [279] 完全平方数
 */

// @lc code=start
class Solution
{
public:
    int numSquares(int n)
    {
        std::vector<int> dp(n + 1, INT_MAX);

        dp[0] = 0;
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j * j <= i; ++j)
            {
                dp[i] = std::min(dp[i - j * j] + 1, dp[i]);
            }
        }

        return dp[n];
    }
};
// @lc code=end
