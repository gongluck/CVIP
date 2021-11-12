/*
 * @lc app=leetcode.cn id=198 lang=cpp
 *
 * [198] 打家劫舍
 */

// @lc code=start
class Solution
{
public:
    /*1*/
    // int dp(const vector<int> &nums, int index)
    // {
    //     if (index >= nums.size())
    //     {
    //         return 0;
    //     }

    //     return std::max(dp(nums, index + 1),
    //                     nums[index] + dp(nums, index + 2));
    // }
    // int rob(vector<int> &nums)
    // {
    //     return dp(nums, 0);
    // }

    /*2*/
    // std::unordered_map<int, int> visited;
    // int dp(const vector<int> &nums, int index)
    // {
    //     if (index >= nums.size())
    //     {
    //         return 0;
    //     }

    //     if (visited.count(index) == 0)
    //     {
    //         visited[index] = std::max(dp(nums, index + 1),
    //                                   nums[index] + dp(nums, index + 2));
    //     }

    //     return visited[index];
    // }
    // int rob(vector<int> &nums)
    // {
    //     return dp(nums, 0);
    // }

    /*3*/
    // int rob(vector<int> &nums)
    // {
    //     std::vector<int> dp(nums.size() + 1, 0);
    //     dp[1] = nums[0];
    //     for (int i = 2; i < nums.size() + 1; ++i)
    //     {
    //         dp[i] = std::max(dp[i - 1],
    //                          nums[i - 1] + dp[i - 2]);
    //     }

    //     return dp[nums.size()];
    // }

    /*4*/
    int rob(vector<int> &nums)
    {
        int dp_first = 0;
        int dp_second = nums[0];
        int dp_cur = dp_second;
        for (int i = 1; i < nums.size(); ++i)
        {
            dp_cur = std::max(dp_second,
                              nums[i] + dp_first);
            dp_first = dp_second;
            dp_second = dp_cur;
        }

        return dp_cur;
    }
};
// @lc code=end
