/*
 * @lc app=leetcode.cn id=322 lang=cpp
 *
 * [322] 零钱兑换
 */

// @lc code=start
class Solution
{
public:
    int res = INT_MAX;
    void dp(vector<int> &coins, int amount, int count)
    {
        if (amount == 0)
        {
            res = std::min(res, count);
            return;
        }
        if (amount < 0)
        {
            return;
        }
        for (int i = 0; i < coins.size(); ++i)
        {
            if (coins[i] == amount)
            {
                res = std::min(res, count + 1);
            }
            else
            {
                dp(coins, amount - coins[i], count + 1);
            }
        }
    }

    int coinChange(vector<int> &coins, int amount)
    {
        dp(coins, amount, 0);
        return res == INT_MAX ? -1 : res;
    }
};
// @lc code=end
