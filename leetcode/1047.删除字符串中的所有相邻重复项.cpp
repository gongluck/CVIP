/*
 * @lc app=leetcode.cn id=1047 lang=cpp
 *
 * [1047] 删除字符串中的所有相邻重复项
 */

// @lc code=start
class Solution
{
public:
    string removeDuplicates(string s)
    {
        std::stack<char> stack;
        for (const auto &c : s)
        {
            if (!stack.empty() && stack.top() == c)
            {
                stack.pop();
            }
            else
            {
                stack.push(c);
            }
        }

        std::string res;
        while (!stack.empty())
        {
            res += stack.top();
            stack.pop();
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};
// @lc code=end
