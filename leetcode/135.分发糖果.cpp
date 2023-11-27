/*
 * @lc app=leetcode.cn id=135 lang=cpp
 *
 * [135] 分发糖果
 */

// @lc code=start
class Solution
{
public:
    int candy(vector<int> &ratings)
    {
        int result = 0;
        int n = ratings.size();
        std::vector<int> nums(n);

        nums[0] = 1;
        for (int i = 1; i < n; ++i)
        {
            nums[i] = ratings[i] > ratings[i - 1] ? nums[i - 1] + 1 : 1;
        }

        result += nums[n - 1];
        for (int i = n - 2; i >= 0; --i)
        {
            nums[i] = ratings[i] > ratings[i + 1] ? std::max(nums[i + 1] + 1, nums[i]) : nums[i];
            result += nums[i];
        }

        return result;
    }
};
// @lc code=end
