/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
class Solution
{
public:
    std::string longest(const std::string &s, int left, int right)
    {
        while (left >= 0 && right < s.size() && s[left] == s[right])
        {
            --left;
            ++right;
        }

        return right - 1 - left > 0 ? s.substr(left + 1, right - 1 - left) : "";
    }

    string longestPalindrome(string s)
    {
        std::string result;

        for (int i = 0; i < s.size(); ++i)
        {
            auto tmp = longest(s, i, i);
            if (tmp.size() > result.size())
            {
                std::swap(result, tmp);
            }
            if (i + 1 < s.size())
            {
                tmp = longest(s, i, i + 1);
                if (tmp.size() > result.size())
                {
                    std::swap(result, tmp);
                }
            }
        }

        return result;
    }
};
// @lc code=end
