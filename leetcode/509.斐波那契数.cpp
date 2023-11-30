/*
 * @lc app=leetcode.cn id=509 lang=cpp
 *
 * [509] 斐波那契数
 */

// @lc code=start
class Solution
{
public:
    int fib(int n)
    {
        if (n <= 1)
        {
            return n;
        }

        int pre2 = 0;
        int pre1 = 1;

        for (int i = 2; i <= n; ++i)
        {
            auto cur = pre2 + pre1;
            pre2 = pre1;
            pre1 = cur;
        }

        return pre1;
    }
};
// @lc code=end
