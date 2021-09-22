/*
 * @lc app=leetcode.cn id=2011 lang=cpp
 *
 * [2011] 执行操作后的变量值
 */

// @lc code=start
class Solution
{
public:
    int finalValueAfterOperations(vector<string> &operations)
    {
        int ret = 0;
        for (const auto &each : operations)
        {
            switch (each[1])
            {
            case '+':
                ++ret;
                break;
            case '-':
                --ret;
                break;
            default:
                break;
            }
        }
        return ret;
    }
};
// @lc code=end
