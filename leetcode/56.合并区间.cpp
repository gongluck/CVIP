/*
 * @lc app=leetcode.cn id=56 lang=cpp
 *
 * [56] 合并区间
 */

// @lc code=start
class Solution
{
public:
    vector<vector<int>> merge(vector<vector<int>> &intervals)
    {
        if (intervals.size() == 0)
        {
            return intervals;
        }
        std::sort(intervals.begin(), intervals.end(),
                  [](const vector<int> &a, const vector<int> &b)
                  { return a[0] < b[0]; });

        vector<vector<int>> result;
        result.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); ++i)
        {
            if (intervals[i][0] <= result.back()[1])
            {
                result.back()[1] = std::max(result.back()[1], intervals[i][1]);
            }
            else
            {
                result.push_back(intervals[i]);
            }
        }
        return result;
    }
};
// @lc code=end
