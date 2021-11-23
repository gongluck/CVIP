/*
 * @lc app=leetcode.cn id=859 lang=cpp
 *
 * [859] 亲密字符串
 */

// @lc code=start
class Solution
{
public:
    bool buddyStrings(string s, string goal)
    {
        //过滤长度不一致和过短的情况
        auto len = s.size();
        if (len < 2 || goal.size() != len)
        {
            return false;
        }

        //记录不同字符数
        int unsame = 0;
        //记录字符出现次数
        std::unordered_map<char, int> charcnts;
        //记录不同的字符
        char u1 = '\0', u2 = '\0';
        for (int i = 0; i < len; ++i)
        {
            ++charcnts[s[i]];
            if (s[i] != goal[i])
            {
                ++unsame;
                if (unsame > 2)
                {
                    return false;
                }

                if (u1 != u2)
                {
                    if (u1 != goal[i] || u2 != s[i])
                    {
                        return false;
                    }
                }
                else
                {
                    u1 = s[i];
                    u2 = goal[i];
                }
            }
        }

        std::cout << unsame << std::endl;
        if (unsame == 0)
        {
            for (const auto &i : charcnts)
            {
                std::cout << i.first << " : " << i.second << std::endl;
                if (i.second > 1)
                {
                    return true;
                }
            }
            return false;
        }
        else if (unsame == 1)
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
