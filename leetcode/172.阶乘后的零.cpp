/*
 * @lc app=leetcode.cn id=172 lang=cpp
 *
 * [172] 阶乘后的零
 */

// @lc code=start
class Solution
{
public:
    int trailingZeroes(int n)
    {
        int result = 0;

        // 统计步长为5的有多少个
        // 统计步长为5*5的有多少个
        // 统计步长为5*5*5的有多少个
        // ...
        while (n /= 5)
        {
            result += n;
        }

        return result;
    }
};
// @lc code=end
