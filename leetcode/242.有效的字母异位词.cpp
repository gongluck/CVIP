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
        std::unordered_map<char, int> charnums;
        for (const auto &c : s)
        {
            ++charnums[c];
        }
        for (const auto &c : t)
        {
            auto it = charnums.find(c);
            if (it == charnums.end())
            {
                return false;
            }
            if (--it->second == 0)
            {
                charnums.erase(it);
            }
        }
        if (charnums.size() > 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};
// @lc code=end
