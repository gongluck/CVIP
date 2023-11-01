/*
 * @lc app=leetcode.cn id=344 lang=cpp
 *
 * [344] 反转字符串
 */

// @lc code=start
class Solution
{
public:
    void reverseString(vector<char> &s)
    {
        int n = s.size();

        for (int i = 0, j = n - 1; i < j; ++i, --j)
        {
            std::swap(s[i], s[j]);
        }

        return;
    }
};
// @lc code=end
