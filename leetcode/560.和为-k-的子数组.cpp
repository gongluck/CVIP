/*
 * @lc app=leetcode.cn id=560 lang=cpp
 *
 * [560] 和为 K 的子数组
 */

// @lc code=start
class Solution
{
public:
    int subarraySum(vector<int> &nums, int k)
    {
        std::unordered_map<int, int> sums;
        sums[0] = 1;

        int sum = 0;
        int count = 0;
        for (int i = 0; i < nums.size(); ++i)
        {
            sum += nums[i];
            count += sums[sum - k];
            sums[sum]++;
        }

        return count;
    }
};
// @lc code=end
