/*
 * @lc app=leetcode.cn id=2938 lang=cpp
 *
 * [2938] 区分黑球与白球
 */

// @lc code=start
class Solution
{
public:
    long long minimumSteps(string s)
    {
        long long count = 0;
        long long result = 0;

        for (const auto &c : s)
        {
            if (c == '1')
            {
                ++count;
            }
            else
            {
                result += count;
            }
        }

        return result;
    }
};
// @lc code=end
