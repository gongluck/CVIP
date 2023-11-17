/*
 * @lc app=leetcode.cn id=53 lang=cpp
 *
 * [53] 最大子数组和
 */

// @lc code=start
class Solution
{
public:
    int maxSubArray(vector<int> &nums)
    {
        int result = INT_MIN;
        int sums = 0;

        for (int i = 0; i < nums.size(); ++i)
        {
            if (sums < 0)
            {
                sums = 0;
            }

            sums += nums[i];
            result = std::max(result, sums);
        }

        return result;
    }
};
// @lc code=end
