/*
 * @lc app=leetcode.cn id=26 lang=cpp
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start
class Solution
{
public:
    int removeDuplicates(vector<int> &nums)
    {
        // int n = nums.size();
        // if (n <= 1)
        // {
        //     return n;
        // }

        // int slow = 1;
        // int fast = 1;

        // while (fast < n)
        // {
        //     if (nums[fast] != nums[fast - 1])
        //     {
        //         nums[slow++] = nums[fast];
        //     }
        //     ++fast;
        // }

        // return slow;

        auto newend = std::unique(nums.begin(), nums.end());
        return newend - nums.begin();
    }
};
// @lc code=end
