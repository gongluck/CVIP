/*
 * @lc app=leetcode.cn id=1024 lang=cpp
 *
 * [1024] 视频拼接
 */

// @lc code=start
class Solution
{
public:
    int videoStitching(vector<vector<int>> &clips, int time)
    {
        std::vector<int> dp(time + 1, INT_MAX - 1);

        dp[0] = 0;
        for (int t = 1; t <= time; ++t)
        {
            for (const auto &clip : clips)
            {
                if (clip[0] <= t && t <= clip[1])
                {
                    dp[t] = std::min(dp[t], dp[clip[0]] + 1);
                }
            }
        }

        return dp[time] == INT_MAX - 1 ? -1 : dp[time];
    }
};
// @lc code=end
