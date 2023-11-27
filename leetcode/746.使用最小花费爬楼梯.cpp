/*
 * @lc app=leetcode.cn id=746 lang=cpp
 *
 * [746] 使用最小花费爬楼梯
 */

// @lc code=start
class Solution
{
public:
    int minCostClimbingStairs(vector<int> &cost)
    {
        int pre2 = 0;
        int pre1 = 0;

        for (int i = 2; i <= cost.size(); ++i)
        {
            int cur = std::min(pre2 + cost[i - 2], pre1 + cost[i - 1]);
            pre2 = pre1;
            pre1 = cur;
        }

        return pre1;
    }
};
// @lc code=end
