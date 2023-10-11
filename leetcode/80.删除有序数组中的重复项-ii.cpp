/*
 * @lc app=leetcode.cn id=80 lang=cpp
 *
 * [80] 删除有序数组中的重复项 II
 */

// @lc code=start
class Solution
{
public:
    int removeDuplicates(vector<int> &nums)
    {
        int n = nums.size();
        if (n <= 2)
        {
            return n;
        }

        int fast = 2;
        int slow = 2;
        while (fast < n)
        {
            if (nums[fast] != nums[slow -2])
            {
                nums[slow++] = nums[fast];
            }
            ++fast;
        }

        return slow;
    }
};
// @lc code=end
