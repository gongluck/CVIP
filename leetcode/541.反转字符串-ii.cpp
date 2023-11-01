/*
 * @lc app=leetcode.cn id=541 lang=cpp
 *
 * [541] 反转字符串 II
 */

// @lc code=start
class Solution
{
public:
    string reverseStr(string s, int k)
    {
        int n = s.size();

        for (int i = 0; i < n; i += 2 * k)
        {
            int tail = i + k;
            std::reverse(s.begin() + i, tail < n ? s.begin() + tail : s.end());
        }

        return s;
    }
};
// @lc code=end
