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
        int result = 0;
        int min = INT_MAX;

        for (const auto &p : prices)
        {
            min = std::min(min, p);
            result = std::max(result, p - min);
        }

        return result;
    }
};
// @lc code=end
