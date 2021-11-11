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
        int sum = 0;
        for (const auto &i : nums)
        {
            sum += i;
        }
        if (sum % 2 == 1)
        {
            return false;
        }

        std::vector<std::vector<bool>> dp(nums.size() + 1, std::vector<bool>(sum + 1, false));
        for (int i = 0; i < nums.size() + 1; ++i)
        {
            dp[i][0] = true;
        }

        for (int i = 1; i < nums.size() + 1; ++i)
        {
            for (int j = 1; j < sum + 1; ++j)
            {
                if (j < nums[i - 1])
                {
                    dp[i][j] = dp[i - 1][j];
                }
                else
                {
                    dp[i][j] = dp[i - 1][j - nums[i - 1]] || dp[i - 1][j];
                }
            }
        }

        return dp[nums.size()][sum / 2];
    }
};
// @lc code=end
