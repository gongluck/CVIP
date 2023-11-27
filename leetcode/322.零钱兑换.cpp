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
        std::vector<int> dp(amount + 1, INT_MAX - 1);

        dp[0] = 0;
        for (auto c : coins)
        {
            for (int s = c; s <= amount; ++s)
            {
                dp[s] = std::min(dp[s], dp[s - c] + 1);
            }
        }

        return dp[amount] == INT_MAX - 1 ? -1 : dp[amount];
    }
};
// @lc code=end
