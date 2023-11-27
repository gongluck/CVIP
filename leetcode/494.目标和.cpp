/*
 * @lc app=leetcode.cn id=494 lang=cpp
 *
 * [494] 目标和
 */

// @lc code=start
class Solution
{
public:
    int findTargetSumWays(vector<int> &nums, int target)
    {
        // x = (target+sum)/2
        target = std::accumulate(nums.begin(), nums.end(), target);
        if (target < 0 || target % 2 == 1)
        {
            return 0;
        }
        target /= 2;

        int n = nums.size();
        std::vector<int> dp(target + 1, 0);

        dp[0] = 1;
        for (const auto &num : nums)
        {
            for (int t = target; t >= num; --t)
            {
                dp[t] += dp[t - num];
            }
        }

        return dp[target];
    }
};
// @lc code=end
