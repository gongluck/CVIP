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
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();
        std::vector<int> dp(n, 0);

        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i == 0 && j == 0)
                {
                    dp[j] = obstacleGrid[i][j] == 1 ? 0 : 1;
                }
                else if (i == 0 && j > 0)
                {
                    dp[j] = (obstacleGrid[i][j] == 1) ? 0 : dp[j - 1];
                }
                else if (i != 0 && j == 0)
                {
                    dp[j] = obstacleGrid[i][j] == 1 ? 0 : dp[j];
                }
                else
                {
                    dp[j] = obstacleGrid[i][j] == 1 ? 0 : dp[j - 1] + dp[j];
                }
            }
        }

        return dp[n - 1];
    }
};
// @lc code=end
