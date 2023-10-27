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
        int n = nums.size();
        int result = INT_MAX;
        int left = 0;
        int right = 0;
        int sum = 0;

        while (right < n)
        {
            sum += nums[right];
            while (sum >= target)
            {
                result = std::min(result, right - left + 1);
                sum -= nums[left++];
            }
            ++right;
        }

        return result == INT_MAX ? 0 : result;
    }
};
// @lc code=end
