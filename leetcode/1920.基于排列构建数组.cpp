/*
 * @lc app=leetcode.cn id=1920 lang=cpp
 *
 * [1920] 基于排列构建数组
 */

// @lc code=start
class Solution
{
public:
    vector<int> buildArray(vector<int> &nums)
    {
        vector<int> result;
        int size = nums.size();
        result.resize(size);
        for (int i = 0; i < size; ++i)
        {
            result[i] = nums[nums[i]];
        }
        return result;
    }
};
// @lc code=end
