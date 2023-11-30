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
        // target = std::accumulate(nums.begin(), nums.end(), target);
        // y = (sum-target)/2
        target = std::accumulate(nums.begin(), nums.end(), -target);
        if (target < 0 || target % 2 == 1)
        {
            return 0;
        }
        target /= 2;

        std::vector<int> dp(target + 1, 0);

        dp[0] = 1;
        for (const auto &num : nums)
        {
            // i的结果依赖前面的结果 所以从后开始遍历
            for (int i = target; i >= num; --i)
            {
                dp[i] += dp[i - num];
            }
        }

        return dp[target];
    }
};
// @lc code=end
