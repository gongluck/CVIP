/*
 * @lc app=leetcode.cn id=125 lang=cpp
 *
 * [125] 验证回文串
 */

// @lc code=start
class Solution
{
public:
    bool isPalindrome(string s)
    {
        int left = 0;
        int right = s.size() - 1;

        while (left < right)
        {
            if (!std::isalnum(s[left]))
            {
                ++left;
                continue;
            }

            if (!std::isalnum(s[right]))
            {
                --right;
                continue;
            }

            if (std::toupper(s[left]) != std::toupper(s[right]))
            {
                return false;
            }

            ++left;
            --right;
        }

        return true;
    }
};
// @lc code=end
