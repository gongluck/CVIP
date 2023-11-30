/*
 * @lc app=leetcode.cn id=63 lang=cpp
 *
 * [63] 不同路径 II
 */

// @lc code=start
class Solution
{
public:
    int uniquePathsWithObstacles(vector<vector<int>> &obstacleGrid)
    {
        // int m = obstacleGrid.size();
        // int n = obstacleGrid[0].size();

        // if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1)
        // {
        //     return 0;
        // }

        // std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

        // for (int i = 0; i < n && obstacleGrid[0][i] != 1; ++i)
        // {
        //     dp[0][i] = 1;
        // }
        // for (int i = 0; i < m && obstacleGrid[i][0] != 1; ++i)
        // {
        //     dp[i][0] = 1;
        // }

        // for (int i = 1; i < m; ++i)
        // {
        //     for (int j = 1; j < n; ++j)
        //     {
        //         dp[i][j] = obstacleGrid[i][j] == 1 ? 0 : dp[i - 1][j] + dp[i][j - 1];
        //     }
        // }

        // return dp[m - 1][n - 1];

        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();

        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1)
        {
            return 0;
        }

        std::vector<int> dp(n, 0);

        for (int i = 0; i < n && obstacleGrid[0][i] != 1; ++i)
        {
            dp[i] = 1;
        }

        for (int i = 1; i < m; ++i)
        {
            dp[0] = obstacleGrid[i][0] == 1 ? 0 : (dp[0]);
            for (int j = 1; j < n; ++j)
            {
                dp[j] = obstacleGrid[i][j] == 1 ? 0 : dp[j] + dp[j - 1];
            }
        }

        return dp[n - 1];
    }
};
// @lc code=end
