/*
 * @lc app=leetcode.cn id=2915 lang=cpp
 *
 * [2915] 和为目标值的最长子序列的长度
 */

// @lc code=start
class Solution
{
public:
    int lengthOfLongestSubsequence(vector<int> &nums, int target)
    {
        std::vector<int> dp(target + 1, INT_MIN);
        int s = 0;

        dp[0] = 0;
        for (auto n : nums)
        {
            s = std::min(s + n, target);
            for (int t = s; t >= n; --t)
            {
                dp[t] = std::max(dp[t - n] + 1, dp[t]);
            }
        }

        return dp[target] > 0 ? dp[target] : -1;
    }
};
// @lc code=end
