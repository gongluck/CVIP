/*
 * @lc app=leetcode.cn id=216 lang=cpp
 *
 * [216] 组合总和 III
 */

// @lc code=start
class Solution
{
public:
    void travel(std::vector<std::vector<int>> &result, std::vector<int> &steps, int k, int n, int start, int sum)
    {
        if (steps.size() == k)
        {
            if (sum == n)
            {
                result.emplace_back(steps);
            }
            return;
        }
        else if (sum >= n)
        {
            return;
        }

        auto maxindex = 9 - (k - steps.size()) + 1;
        for (int i = start; i <= maxindex; ++i)
        {
            steps.push_back(i);
            travel(result, steps, k, n, i + 1, sum + i);
            steps.pop_back();
        }

        return;
    }

    vector<vector<int>> combinationSum3(int k, int n)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        travel(result, steps, k, n, 1, 0);

        return result;
    }
};
// @lc code=end
