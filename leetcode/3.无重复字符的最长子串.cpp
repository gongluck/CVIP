/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        int left = 0;
        int right = 0;
        std::map<char, int> window;
        int res = 0;

        while (right < s.size())
        {
            ++window[s[right++]];

            while(window[s[right-1]] > 1)
            {
                --window[s[left++]];
            }

            res = std::max(res, right - left);
        }

        return res;
    }
};
// @lc code=end
