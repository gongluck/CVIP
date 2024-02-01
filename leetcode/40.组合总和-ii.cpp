/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
 */

// @lc code=start
class Solution
{
public:
    void combin(std::vector<int> &candidates, int index, std::vector<int> &steps, int target, int sum, std::vector<std::vector<int>> &result)
    {
        if (sum == target)
        {
            result.emplace_back(steps);
            return;
        }

        for (int i = index; i < candidates.size(); ++i)
        {
            if (i > index && candidates[i] == candidates[i - 1])
            {
                continue;
            }

            if (sum + candidates[i] > target)
            {
                break;
            }

            steps.push_back(candidates[i]);
            combin(candidates, i + 1, steps, target, sum + candidates[i], result);
            steps.pop_back();
        }
    }

    vector<vector<int>> combinationSum2(vector<int> &candidates, int target)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        std::sort(candidates.begin(), candidates.end());

        combin(candidates, 0, steps, target, 0, result);

        return result;
    }
};
// @lc code=end
