/*
 * @lc app=leetcode.cn id=739 lang=cpp
 *
 * [739] 每日温度
 */

// @lc code=start
class Solution
{
public:
    vector<int> dailyTemperatures(vector<int> &temperatures)
    {
        std::vector<int> res(temperatures.size(), 0);
        std::stack<int> stack;
        for (int i = 0; i < temperatures.size(); ++i)
        {
            while (!stack.empty() && temperatures[stack.top()] < temperatures[i])
            {
                res[stack.top()] = i - stack.top();
                stack.pop();
            }
            stack.push(i);
        }

        return res;
    }
};
// @lc code=end
