/*
 * @lc app=leetcode.cn id=921 lang=cpp
 *
 * [921] 使括号有效的最少添加
 */

// @lc code=start
class Solution
{
public:
    int minAddToMakeValid(string s)
    {
        std::stack<char> stack;
        for (const auto &i : s)
        {
            if (i == '(')
            {
                stack.push('(');
            }
            else if (i == ')')
            {
                if (!stack.empty() && stack.top() == '(')
                {
                    stack.pop();
                }
                else
                {
                    stack.push(')');
                }
            }
        }
        return stack.size();
    }
};
// @lc code=end
