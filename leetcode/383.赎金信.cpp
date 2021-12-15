/*
 * @lc app=leetcode.cn id=383 lang=cpp
 *
 * [383] 赎金信
 */

// @lc code=start
class Solution
{
public:
    bool canConstruct(string ransomNote, string magazine)
    {
        std::unordered_map<char, int> charnums;
        for (const auto &c : magazine)
        {
            ++charnums[c];
        }
        for (const auto &c : ransomNote)
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
        return true;
    }
};
// @lc code=end
