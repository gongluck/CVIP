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
        //[left, right)
        size_t left = 0;
        while (left < s.size())
        {
            size_t right = std::min(left + k, s.size());
            //std::cout << left << " " << right << std::endl;
            std::reverse(s.begin() + left, s.begin() + right);
            left += 2*k;
        }
        return s;
    }
};
// @lc code=end
