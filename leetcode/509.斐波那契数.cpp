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
        if(n == 0)
        {
            return 0;
        }
        else if(n == 1 || n == 2)
        {
            return 1;
        }

        int n1 = 0;
        int n2 = 1;
        for (int i = 2; i <= n; ++i)
        {
            n2 = n1 + n2;
            n1 = n2 - n1;
            n1 >= 1;
        }
        return n2;
    }
};
// @lc code=end
