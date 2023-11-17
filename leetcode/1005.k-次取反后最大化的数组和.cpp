/*
 * @lc app=leetcode.cn id=1005 lang=cpp
 *
 * [1005] K 次取反后最大化的数组和
 */

// @lc code=start
class Solution
{
public:
    int largestSumAfterKNegations(vector<int> &nums, int k)
    {
        int result = 0;
        int index = 0;

        std::sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i)
        {
            if (k > 0 && nums[i] <= 0)
            {
                nums[i] = -nums[i];
                --k;
                index = i;
            }
            result += nums[i];
        }

        if (k % 2 == 1)
        {
            result -= 2 * (index + 1 < nums.size() ? std::min(nums[index], nums[index + 1]) : nums[index]);
        }

        return result;
    }
};
// @lc code=end
