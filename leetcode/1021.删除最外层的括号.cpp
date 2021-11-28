/*
 * @lc app=leetcode.cn id=1021 lang=cpp
 *
 * [1021] 删除最外层的括号
 */

// @lc code=start
class Solution
{
public:
    string removeOuterParentheses(string s)
    {
        std::string res;
        int lcount = 0;
        int rcount = 0;
        int start = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            if (s[i] == '(')
            {
                ++lcount;
            }
            else if (s[i] == ')')
            {
                ++rcount;
                if (lcount == rcount)
                {
                    res += s.substr(start + 1, i - start - 1);
                    lcount = rcount = 0;
                    start = i + 1;
                }
            }
        }
        return res;
    }
};
// @lc code=end
