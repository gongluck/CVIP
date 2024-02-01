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
        int result = 0;
        int left = 0;
        int right = 0;
        bool exist[128] = {false};

        while (right < s.size())
        {
            if (exist[s[right]])
            {
                while (s[left] != s[right])
                {
                    exist[s[left]] = false;
                    ++left;
                }
                ++left;
            }
            exist[s[right]] = true;

            result = std::max(result, right - left + 1);

            ++right;
        }

        return result;
    }
};
// @lc code=end
