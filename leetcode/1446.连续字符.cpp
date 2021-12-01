/*
 * @lc app=leetcode.cn id=1446 lang=cpp
 *
 * [1446] 连续字符
 */

// @lc code=start
class Solution
{
public:
    int maxPower(string s)
    {
        char cur = '\0';
        int res = INT_MIN;
        int count = 0;
        for (const auto &i : s)
        {
            if (cur == i)
            {
                ++count;
            }
            else
            {
                res = std::max(res, count);
                cur = i;
                count = 1;
            }
        }

        res = std::max(res, count);
        return res;
    }
};
// @lc code=end
