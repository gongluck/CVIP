/*
 * @lc app=leetcode.cn id=2909 lang=cpp
 *
 * [2909] 元素和最小的山形三元组 II
 */

// @lc code=start
class Solution
{
public:
    int minimumSum(vector<int> &nums)
    {
        int result = INT_MAX;
        int n = nums.size();
        std::vector<int> sufmin(n);
        sufmin[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; --i)
        {
            sufmin[i] = std::min(nums[i], sufmin[i + 1]);
        }

        int premin = nums[0];
        for (int i = 1; i < n - 1; ++i)
        {
            if (nums[i] > premin && nums[i] > sufmin[i + 1])
            {
                result = std::min(result, nums[i] + premin + sufmin[i + 1]);
            }
            premin = std::min(premin, nums[i]);
        }

        return result == INT_MAX ? -1 : result;
    }
};
// @lc code=end
