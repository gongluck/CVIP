/*
 * @lc app=leetcode.cn id=150 lang=cpp
 *
 * [150] 逆波兰表达式求值
 */

// @lc code=start
class Solution
{
public:
    int evalRPN(vector<string> &tokens)
    {
        std::stack<int> stack;
        for (const auto &s : tokens)
        {
            if (s == "+" ||
                s == "-" ||
                s == "*" ||
                s == "/")
            {
                auto num1 = stack.top();
                stack.pop();
                auto num2 = stack.top();
                stack.pop();
                if (s == "+")
                {
                    stack.push(num1 + num2);
                }
                else if (s == "-")
                {
                    stack.push(num2 - num1);
                }
                else if (s == "*")
                {
                    stack.push(num1 * num2);
                }
                else if (s == "/")
                {
                    stack.push(num2 / num1);
                }
            }
            else
            {
                stack.push(std::stoi(s));
            }
        }
        return stack.top();
    }
};
// @lc code=end
