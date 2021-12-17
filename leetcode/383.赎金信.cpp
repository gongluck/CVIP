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
        // std::unordered_map<char, int> charnums;
        // for (const auto &c : magazine)
        // {
        //     ++charnums[c];
        // }
        // for (const auto &c : ransomNote)
        // {
        //     if (charnums.count(c) > 0)
        //     {
        //         --charnums[c];
        //         if (charnums[c] == 0)
        //         {
        //             charnums.erase(c);
        //         }
        //     }
        //     else
        //     {
        //         return false;
        //     }
        // }
        // return true;

        //数组优化
        int cns[26] = {0};
        for (const auto &c : magazine)
        {
            ++cns[c - 'a'];
        }
        for (const auto &c : ransomNote)
        {
            --cns[c - 'a'];
            if (cns[c - 'a'] < 0)
            {
                return false;
            }
        }
        return true;
    }
};
// @lc code=end
