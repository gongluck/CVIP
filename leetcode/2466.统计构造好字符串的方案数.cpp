/*
 * @lc app=leetcode.cn id=2466 lang=cpp
 *
 * [2466] 统计构造好字符串的方案数
 */

// @lc code=start
class Solution
{
public:
    int countGoodStrings(int low, int high, int zero, int one)
    {
        int result = 0;
        std::vector<int> nums(high + 1);
        const long long MOD = std::pow(10, 9) + 7;

        nums[0] = 1; // 空串方案数
        for (int i = 0; i <= high; ++i)
        {
            if (i >= zero)
            {
                nums[i] += nums[i - zero];
                nums[i] %= MOD;
            }
            if (i >= one)
            {
                nums[i] += nums[i - one];
                nums[i] %= MOD;
            }
            if (i >= low)
            {
                result += nums[i];
                result %= MOD;
            }
        }

        return result;
    }
};
// @lc code=end
