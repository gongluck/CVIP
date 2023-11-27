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
        int pre2 = 1;
        int pre1 = 1;
        int cur = 1;

        for (int i = 1; i < n; ++i)
        {
            cur = pre1 + pre2;
            pre2 = pre1;
            pre1 = cur;
        }

        return cur;
    }
};
// @lc code=end
