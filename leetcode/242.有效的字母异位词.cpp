/*
 * @lc app=leetcode.cn id=242 lang=cpp
 *
 * [242] 有效的字母异位词
 */

// @lc code=start
class Solution
{
public:
    bool isAnagram(string s, string t)
    {
        if (s.size() != t.size())
        {
            return false;
        }

        int sets[26] = {0};
        for (const auto &i : s)
        {
            ++sets[i - 'a'];
        }
        for (const auto &i : t)
        {
            if (sets[i - 'a']-- <= 0)
            {
                return false;
            }
        }

        return true;
    }
};
// @lc code=end
