/*
 * @lc app=leetcode.cn id=1929 lang=cpp
 *
 * [1929] 数组串联
 */

// @lc code=start
class Solution
{
public:
    vector<int> getConcatenation(vector<int> &nums)
    {
        int size = nums.size();
        nums.resize(size * 2);
        memcpy(&nums[size], &nums[0], sizeof(int) * size);
        return nums;
    }
};
// @lc code=end
