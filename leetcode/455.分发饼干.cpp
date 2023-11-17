/*
 * @lc app=leetcode.cn id=455 lang=cpp
 *
 * [455] 分发饼干
 */

// @lc code=start
class Solution
{
public:
    int findContentChildren(vector<int> &g, vector<int> &s)
    {
        int result = 0;

        std::sort(g.begin(), g.end());
        std::sort(s.begin(), s.end());

        for (int i = 0, j = 0; i < g.size() && j < s.size(); ++i)
        {
            while (j < s.size())
            {
                if (g[i] <= s[j++])
                {
                    ++result;
                    break;
                }
            }
        }

        return result;
    }
};
// @lc code=end
