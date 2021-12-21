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
        //库函数
        // std::reverse(s.begin(), s.end());

        int n = s.size();
        for (int left = 0, right = n - 1; left < right; ++left, --right)
        {
            std::swap(s[left], s[right]);
        }
    }
};
// @lc code=end
