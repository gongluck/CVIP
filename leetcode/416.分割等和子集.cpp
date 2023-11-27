/*
 * @lc app=leetcode.cn id=416 lang=cpp
 *
 * [416] 分割等和子集
 */

// @lc code=start
class Solution
{
public:
    bool canPartition(vector<int> &nums)
    {
        int target = std::accumulate(nums.begin(), nums.end(), 0);
        if (target % 2 == 1)
        {
            return false;
        }
        target /= 2;

        std::vector<bool> dp(target + 1, false);
        dp[0] = true;
        for (auto n : nums)
        {
            for (int t = target; t >= n; --t)
            {
                dp[t] = dp[t] || dp[t - n];
            }
        }

        return dp[target];
    }
};
// @lc code=end
