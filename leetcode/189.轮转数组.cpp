/*
 * @lc app=leetcode.cn id=189 lang=cpp
 *
 * [189] 轮转数组
 */

// @lc code=start
class Solution
{
public:
    void rotate(vector<int> &nums, int k)
    {
        // std::reverse(nums.begin(), nums.end());
        // std::reverse(nums.begin(), nums.begin() + k);
        // std::reverse(nums.begin() + k, nums.end());

        int n = nums.size();
        k %= n;
        if (k == 0)
        {
            return;
        }

        for (int left = 0, right = n - 1; left < right; ++left, --right)
        {
            std::swap(nums[left], nums[right]);
        }

        for (int left = 0, right = k - 1; left < right; ++left, --right)
        {
            std::swap(nums[left], nums[right]);
        }

        for (int left = k, right = n - 1; left < right; ++left, --right)
        {
            std::swap(nums[left], nums[right]);
        }
    }
};
// @lc code=end
