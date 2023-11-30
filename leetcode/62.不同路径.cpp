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
        // std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

        // for (int i = 0; i < n; ++i)
        // {
        //     dp[0][i] = 1;
        // }

        // for (int i = 1; i < m; ++i)
        // {
        //     dp[i][0] = 1;
        //     for (int j = 1; j < n; ++j)
        //     {
        //         dp[i][j] = dp[i - 1][j] /*左边*/ + dp[i][j - 1] /*上边*/;
        //     }
        // }

        // return dp[m - 1][n - 1];

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
