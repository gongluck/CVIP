/*
 * @lc app=leetcode.cn id=46 lang=cpp
 *
 * [46] 全排列
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
            if (selected[i])
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

    vector<vector<int>> permute(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;
        std::vector<bool> selected(nums.size(), false);

        perm(result, nums, selected, steps);

        return result;
    }
};
// @lc code=end
