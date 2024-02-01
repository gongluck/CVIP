/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
class Solution
{
public:
    // std::string longest(const std::string &s, int left, int right)
    // {
    //     while (left >= 0 && right < s.size() && s[left] == s[right])
    //     {
    //         --left;
    //         ++right;
    //     }

    //     return right - left - 1 > 0 ? s.substr(left + 1, right - left - 1) : "";
    // }

    // string longestPalindrome(string s)
    // {
    //     std::string result;

    //     for (int i = 0; i < s.size(); ++i)
    //     {
    //         auto tmp = longest(s, i, i);
    //         if (tmp.size() > result.size())
    //         {
    //             result = tmp;
    //         }
    //         tmp = longest(s, i, i + 1);
    //         if (tmp.size() > result.size())
    //         {
    //             result = tmp;
    //         }
    //     }
    //     return result;
    // }
    string longestPalindrome(string s)
    {
        int start = 0;
        int len = 1;
        int strlen = s.size();
        std::vector<std::vector<bool>> dp(strlen, std::vector<bool>(strlen, false));

        for (int i = 0; i < strlen; ++i)
        {
            dp[i][i] = true;
            if (i < strlen - 1 && s[i] == s[i + 1])
            {
                dp[i][i + 1] = true;
                start = i;
                len = 2;
            }
        }

        for (int l = 3; l <= strlen; ++l)
        {
            for (int i = 0; i + l - 1 < strlen; ++i)
            {
                int j = i + l - 1;
                if (s[i] == s[j] && dp[i + 1][j - 1])
                {
                    dp[i][j] = true;
                    start = i;
                    len = l;
                }
            }
        }

        return s.substr(start, len);
    }
};
// @lc code=end
