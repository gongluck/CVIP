/*
 * @lc app=leetcode.cn id=491 lang=cpp
 *
 * [491] 递增子序列
 */

// @lc code=start
class Solution
{
public:
    void findSub(std::vector<std::vector<int>> &result, std::vector<int> &steps, const std::vector<int> &nums, int index)
    {
        if (index > nums.size())
        {
            return;
        }

        if (steps.size() >= 2)
        {
            result.emplace_back(steps);
        }

        std::unordered_set<int> sets;
        for (int i = index; i < nums.size(); ++i)
        {
            if ((steps.size() > 0 && steps[steps.size() - 1] > nums[i]) || sets.count(nums[i]) > 0)
            {
                continue;
            }
            sets.insert(nums[i]);

            steps.push_back(nums[i]);
            findSub(result, steps, nums, i + 1);
            steps.pop_back();
        }
    }

    vector<vector<int>> findSubsequences(vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        findSub(result, steps, nums, 0);

        return result;
    }
};
// @lc code=end
