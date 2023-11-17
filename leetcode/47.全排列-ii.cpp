/*
 * @lc app=leetcode.cn id=47 lang=cpp
 *
 * [47] 全排列 II
 */

// @lc code=start
class Solution
{
public:
    void perm(std::vector<std::vector<int>> &result, const std::vector<int> &nums, std::vector<bool> &selected, std::vector<int> &steps)
    {
        if (steps.size() == nums.size())
        {
            result.emplace_back(steps);
            return;
        }

        for (int i = 0; i < nums.size(); ++i)
        {
            if (selected[i] || (i > 0 && nums[i] == nums[i - 1] && !selected[i - 1])) // !selected[i-1] 代表还在同一层
            {
                continue;
            }

            steps.push_back(nums[i]);
            selected[i] = true;
            perm(result, nums, selected, steps);
            selected[i] = false;
            steps.pop_back();
        }
    }

    vector<vector<int>> permuteUnique(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;
        std::vector<bool> selected(nums.size(), false);

        std::sort(nums.begin(), nums.end());
        perm(result, nums, selected, steps);

        return result;
    }
};
// @lc code=end
