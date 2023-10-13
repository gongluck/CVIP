/*
 * @lc app=leetcode.cn id=121 lang=cpp
 *
 * [121] 买卖股票的最佳时机
 */

// @lc code=start
class Solution
{
public:
    int maxProfit(vector<int> &prices)
    {
        int n = prices.size();
        int min = INT_MAX;
        int max = INT_MIN;
        for (int i = 1; i < n; ++i)
        {
            min = std::min(min, prices[i - 1]);
            max = std::max(max, prices[i] - min);
        }

        return std::max(0, max);
    }
};
// @lc code=end
