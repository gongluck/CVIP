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
        std::stack<char> st;

        for (const auto &c : s)
        {
            switch (c)
            {
            case '(':
            case '[':
            case '{':
                st.push(c);
                break;
            case ')':
                if (st.empty() || st.top() != '(')
                {
                    return false;
                }
                st.pop();
                break;
            case ']':
                if (st.empty() || st.top() != '[')
                {
                    return false;
                }
                st.pop();
                break;
            case '}':
                if (st.empty() || st.top() != '{')
                {
                    return false;
                }
                st.pop();
                break;
            }
        }

        return st.empty();
    }
};
// @lc code=end
