/*
 * @lc app=leetcode.cn id=46 lang=cpp
 *
 * [46] 全排列
 */

// @lc code=start
class Solution
{
public:
    void perm(const std::vector<int> &nums, std::vector<int> &steps,
              std::unordered_map<int, bool> &selected,
              std::vector<std::vector<int>> &result)
    {
        if (steps.size() == nums.size())
        {
            result.push_back(steps);
            return;
        }
        for (const auto &n : nums)
        {
            if (selected[n])
            {
                continue;
            }
            // std::cout << "select: " << n << std::endl;
            steps.push_back(n);
            selected[n] = true;
            perm(nums, steps, selected, result);
            steps.pop_back();
            selected[n] = false;
        }
    }

    vector<vector<int>> permute(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::unordered_map<int, bool> selected;
        std::vector<int> steps;

        perm(nums, steps, selected, result);

        return result;
    }
};
// @lc code=end
