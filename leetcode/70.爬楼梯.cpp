/*
 * @lc app=leetcode.cn id=70 lang=cpp
 *
 * [70] 爬楼梯
 */

// @lc code=start
class Solution
{
public:
    int climbStairs(int n)
    {
        if (n <= 2)
        {
            return n;
        }

        int pre2 = 1;
        int pre1 = 2;

        for (int i = 3; i <= n; ++i)
        {
            auto cur = pre2 + pre1;
            pre2 = pre1;
            pre1 = cur;
        }

        return pre1;
    }
};
// @lc code=end
