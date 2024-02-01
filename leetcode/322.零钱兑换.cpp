/*
 * @lc app=leetcode.cn id=322 lang=cpp
 *
 * [322] 零钱兑换
 */

// @lc code=start
class Solution
{
public:
    int coinChange(vector<int> &coins, int amount)
    {
        std::vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;
        for (const auto &c : coins)
        {
            for (int m = c; m <= amount; ++m)
            {
                dp[m] = std::min(dp[m],
                                 dp[m - c] != INT_MAX ? dp[m - c] + 1 : INT_MAX);
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};
// @lc code=end
