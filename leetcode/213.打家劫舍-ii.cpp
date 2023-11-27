/*
 * @lc app=leetcode.cn id=213 lang=cpp
 *
 * [213] 打家劫舍 II
 */

// @lc code=start
class Solution
{
public:
    int robb(const std::vector<int> &nums, int start, int end)
    {
        int pre2 = 0;
        int pre1 = 0;

        for (int i = start; i < end; ++i)
        {
            auto cur = std::max(pre2 + nums[i], pre1);
            pre2 = pre1;
            pre1 = cur;
        }

        return pre1;
    }

    int rob(vector<int> &nums)
    {
        return std::max(nums[0] + robb(nums, 2, nums.size() - 1),
                        robb(nums, 1, nums.size()));
    }
};
// @lc code=end
