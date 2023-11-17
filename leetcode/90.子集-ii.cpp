/*
 * @lc app=leetcode.cn id=90 lang=cpp
 *
 * [90] 子集 II
 */

// @lc code=start
class Solution
{
public:
    void sub(std::vector<std::vector<int>> &result, std::vector<int> &steps, const vector<int> &nums, int index)
    {
        if (index > nums.size())
        {
            return;
        }

        result.emplace_back(steps);

        for (int i = index; i < nums.size(); ++i)
        {
            if (i > index && nums[i] == nums[i - 1])
            {
                continue;
            }
            steps.push_back(nums[i]);
            sub(result, steps, nums, i + 1);
            steps.pop_back();
        }
    }

    vector<vector<int>> subsetsWithDup(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        std::sort(nums.begin(), nums.end());
        sub(result, steps, nums, 0);

        return result;
    }
};
// @lc code=end
