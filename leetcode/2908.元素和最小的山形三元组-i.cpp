/*
 * @lc app=leetcode.cn id=2908 lang=cpp
 *
 * [2908] 元素和最小的山形三元组 I
 */

// @lc code=start
class Solution
{
public:
    int minimumSum(vector<int> &nums)
    {
        int result = INT_MAX;
        int n = nums.size();

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                for (int k = j + 1; k < n; ++k)
                {
                    if (nums[j] > nums[i] && nums[j] > nums[k])
                    {
                        result = std::min(result, nums[i] + nums[j] + nums[k]);
                    }
                }
            }
        }

        return result == INT_MAX ? -1 : result;
    }
};
// @lc code=end
