/*
 * @lc app=leetcode.cn id=209 lang=cpp
 *
 * [209] 长度最小的子数组
 */

// @lc code=start
class Solution
{
public:
    int minSubArrayLen(int target, vector<int> &nums)
    {
        int result = INT_MAX;
        int n = nums.size();
        int left = 0;
        int right = 0;
        int sums = 0;

        while (right < n)
        {
            sums += nums[right++];
            while (sums >= target) // 因为是正整数序列，所以不用判断left<right
            {
                result = std::min(right - left, result);
                sums -= nums[left++];
            };
        }

        return result == INT_MAX ? 0 : result;
    }
};
// @lc code=end
