/*
 * @lc app=leetcode.cn id=2400 lang=cpp
 *
 * [2400] 恰好移动 k 步到达某一位置的方法数目
 */

// @lc code=start
class Solution
{
public:
    int numberOfWays(int startPos, int endPos, int k)
    {
        int distance = abs(startPos - endPos);
        if (k < distance)
        {
            return 0;
        }

        std::vector<std::vector<long long>> dp(k + 1, std::vector<long long>(k + 1, 0));
        for (int i = 0; i <= k; ++i)
        {
            dp[i][i] = 1;
        }

        for (int i = 1; i <= k; ++i)
        {
            dp[i][0] = 2 * dp[i - 1][1];
            for (int j = 1; j < i; ++j)
            {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j + 1];
                dp[i][j] %= (1000000007);
            }
        }

        return dp[k][distance] %= (1000000007);
    }
};
// @lc code=end
