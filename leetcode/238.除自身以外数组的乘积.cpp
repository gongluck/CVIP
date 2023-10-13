/*
 * @lc app=leetcode.cn id=238 lang=cpp
 *
 * [238] 除自身以外数组的乘积
 */

// @lc code=start
class Solution
{
public:
    vector<int> productExceptSelf(vector<int> &nums)
    {
        int n = nums.size();
        std::vector<int> result(n, 1);
        // 前缀乘积
        for (int i = 1, j = 1; i <= n; ++i)
        {
            result[i - 1] = j;
            j *= nums[i - 1];
        }
        // 前缀乘积 x 后缀乘积
        for (int i = n, j = 1; i >= 1; --i)
        {
            result[i - 1] *= j;
            j *= nums[i - 1];
        }
        return result;
    }
};
// @lc code=end
