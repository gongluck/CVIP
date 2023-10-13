/*
 * @lc app=leetcode.cn id=274 lang=cpp
 *
 * [274] H 指数
 */

// @lc code=start
class Solution
{
public:
    int hIndex(vector<int> &citations)
    {
        std::sort(citations.begin(), citations.end());
        int h = 0;
        for (int i = citations.size() - 1; i >= 0; --i)
        {
            if (citations[i] <= h)
            {
                break;
            }
            ++h;
        }
        return h;
    }
};
// @lc code=end
