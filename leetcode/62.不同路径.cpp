/*
 * @lc app=leetcode.cn id=62 lang=cpp
 *
 * [62] 不同路径
 */

// @lc code=start
class Solution
{
public:
    int uniquePaths(int m, int n)
    {
        std::vector<int> dp(n, 1);

        for (int i = 1; i < m; ++i)
        {
            for (int j = 1; j < n; ++j)
            {
                dp[j] += dp[j - 1];
            }
        }

        return dp[n - 1];
    }
};
// @lc code=end
