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
        std::stack<int> st;

        for (const auto &s : tokens)
        {
            if (s == "+")
            {
                auto n2 = st.top();
                st.pop();
                auto n1 = st.top();
                st.pop();
                st.push(n1 + n2);
            }
            else if (s == "-")
            {
                auto n2 = st.top();
                st.pop();
                auto n1 = st.top();
                st.pop();
                st.push(n1 - n2);
            }
            else if (s == "*")
            {
                auto n2 = st.top();
                st.pop();
                auto n1 = st.top();
                st.pop();
                st.push(n1 * n2);
            }
            else if (s == "/")
            {
                auto n2 = st.top();
                st.pop();
                auto n1 = st.top();
                st.pop();
                st.push(n1 / n2);
            }
            else
            {
                st.push(std::atoi(s.c_str()));
            }
        }

        return st.top();
    }
};
// @lc code=end
