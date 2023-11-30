/*
 * @lc app=leetcode.cn id=343 lang=cpp
 *
 * [343] 整数拆分
 */

// @lc code=start
class Solution
{
public:
    int integerBreak(int n)
    {
        std::vector<int> dp(n + 1, 0);

        dp[2] = 1;

        for (int i = 3; i <= n; ++i)
        {
            for (int j = 2; j <= i / 2; ++j)
            {
                dp[i] = std::max(dp[i], std::max((i - j) * j, j * dp[i - j] /*j+x...*/));
            }
        }

        return dp[n];
    }
};
// @lc code=end
