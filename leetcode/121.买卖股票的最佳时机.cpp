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
        int min = INT_MAX;
        int max = INT_MIN;

        for (const auto &price : prices)
        {
            min = std::min(min, price);
            max = std::max(max, price - min);
        }

        return max;
    }
};
// @lc code=end
