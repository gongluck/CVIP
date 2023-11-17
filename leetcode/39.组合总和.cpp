/*
 * @lc app=leetcode.cn id=39 lang=cpp
 *
 * [39] 组合总和
 */

// @lc code=start
class Solution
{
public:
    void combin(std::vector<std::vector<int>> &result, const std::vector<int> &candidates, int target, std::vector<int> &steps, int sums, int index)
    {
        if (sums == target)
        {
            result.emplace_back(steps);
            return;
        }

        for (int i = index; i < candidates.size(); ++i)
        {
            if (sums + candidates[i] > target)
            {
                break;
            }

            steps.push_back(candidates[i]);
            combin(result, candidates, target, steps, sums + candidates[i], i);
            steps.pop_back();
        }
    }

    vector<vector<int>> combinationSum(vector<int> &candidates, int target)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        std::sort(candidates.begin(), candidates.end());
        combin(result, candidates, target, steps, 0, 0);

        return result;
    }
};
// @lc code=end
