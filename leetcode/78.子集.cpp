/*
 * @lc app=leetcode.cn id=78 lang=cpp
 *
 * [78] 子集
 */

// @lc code=start
class Solution
{
public:
    void sub(std::vector<std::vector<int>> &result, std::vector<int> &steps, const std::vector<int> &nums, int index)
    {
        if (index > nums.size())
        {
            return;
        }

        result.emplace_back(steps);

        for (int i = index; i < nums.size(); ++i)
        {
            steps.push_back(nums[i]);
            sub(result, steps, nums, i + 1);
            steps.pop_back();
        }
    }

    vector<vector<int>> subsets(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        sub(result, steps, nums, 0);

        return result;
    }
};
// @lc code=end
