/*
 * @lc app=leetcode.cn id=1614 lang=cpp
 *
 * [1614] 括号的最大嵌套深度
 */

// @lc code=start
class Solution
{
public:
    int maxDepth(string s)
    {
        int res = 0;
        int lcount = 0;
        for (const auto &i : s)
        {
            if (i == '(')
            {
                ++lcount;
            }
            else if (i == ')')
            {
                res = std::max(res, lcount);
                --lcount;
            }
        }

        return res;
    }
};
// @lc code=end
