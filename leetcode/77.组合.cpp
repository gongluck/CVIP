/*
 * @lc app=leetcode.cn id=77 lang=cpp
 *
 * [77] 组合
 */

// @lc code=start
class Solution
{
public:
    void travel(std::vector<std::vector<int>> &result, std::vector<int> &steps, int start, int n, int k)
    {
        if (steps.size() == k)
        {
            result.emplace_back(steps);
            return;
        }

        auto maxindex = n - (k - steps.size()) + 1;
        for (int i = start; i <= maxindex; ++i)
        {
            if (k - steps.size() > n - i + 1)
            {
                break;
            }
            steps.push_back(i);
            travel(result, steps, i + 1, n, k);
            steps.pop_back();
        }
    }

    vector<vector<int>> combine(int n, int k)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> steps;

        travel(result, steps, 1, n, k);
        return result;
    }
};
// @lc code=end
