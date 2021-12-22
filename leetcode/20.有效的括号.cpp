/*
 * @lc app=leetcode.cn id=20 lang=cpp
 *
 * [20] 有效的括号
 */

// @lc code=start
class Solution
{
public:
    bool isValid(string s)
    {
        std::stack<int> stack;
        for (const auto &i : s)
        {
            switch (i)
            {
            case '(':
            case '[':
            case '{':
                stack.push(i);
                break;
            case ')':
                if (stack.empty() || stack.top() != '(')
                {
                    return false;
                }
                stack.pop();
                break;
            case ']':
                if (stack.empty() || stack.top() != '[')
                {
                    return false;
                }
                stack.pop();
                break;
            case '}':
                if (stack.empty() || stack.top() != '{')
                {
                    return false;
                }
                stack.pop();
                break;
            }
        }
        return stack.empty();
    }
};
// @lc code=end
