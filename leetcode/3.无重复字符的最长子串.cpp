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
        int n = s.size();
        int left = 0;
        int right = 0;
        std::unordered_set<char> sets;

        for (; right < n; ++right)
        {
            while (sets.count(s[right]) != 0)
            {
                sets.erase(s[left++]);
            }
            result = std::max(result, right - left + 1);
            sets.insert(s[right]);
        }

        return result;
    }
};
// @lc code=end
