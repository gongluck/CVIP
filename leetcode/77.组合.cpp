/*
 * @lc app=leetcode.cn id=77 lang=cpp
 *
 * [77] 组合
 */

// @lc code=start
class Solution
{
public:
    vector<vector<int>> res;
    void dp(int n, int k, int pos, vector<int> &track)
    {
        if (track.size() == k)
        {
            res.push_back(track);
            return;
        }

        for (int i = pos; i <= n; ++i)
        {
            track.push_back(i);
            dp(n, k, i + 1, track);
            track.pop_back();
        }
    }
    vector<vector<int>> combine(int n, int k)
    {
        vector<int> track;
        dp(n, k, 1, track);
        return res;
    }
};
// @lc code=end
