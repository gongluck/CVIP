/*
 * @lc app=leetcode.cn id=594 lang=cpp
 *
 * [594] 最长和谐子序列
 */

// @lc code=start
class Solution
{
public:
    int findLHS(vector<int> &nums)
    {
        unordered_map<int, int> cnt;
        int res = 0;
        for (int num : nums)
        {
            cnt[num]++;
        }
        for (auto [key, val] : cnt)
        {
            if (cnt.count(key + 1))
            {
                res = max(res, val + cnt[key + 1]);
            }
        }
        return res;
    }
};
// @lc code=end
