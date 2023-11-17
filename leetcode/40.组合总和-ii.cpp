/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
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
            if (i > index /*不是这一层的第一个数*/ && candidates[i] == candidates[i - 1])
            {
                continue;
            }

            steps.push_back(candidates[i]);
            combin(result, candidates, target, steps, sums + candidates[i], i + 1);
            steps.pop_back();
        }
    }

    vector<vector<int>> combinationSum2(vector<int> &candidates, int target)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        std::sort(candidates.begin(), candidates.end());
        combin(result, candidates, target, steps, 0, 0);

        return result;
    }
};
// @lc code=end
