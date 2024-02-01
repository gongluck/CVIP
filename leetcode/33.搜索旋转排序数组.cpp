/*
 * @lc app=leetcode.cn id=33 lang=cpp
 *
 * [33] 搜索旋转排序数组
 */

// @lc code=start
class Solution
{
public:
    int search(vector<int> &nums, int target)
    {
        int n = nums.size();
        int l = 0;
        int r = n;
        int mid;

        if (target < nums[0])
        {
            target += 1001;
        }
        while (l < r)
        {
            mid = (l + r) / 2;
            if (nums[mid] < nums[0])
            {
                nums[mid] += 1001;
            }
            if (nums[mid] == target)
            {
                return mid;
            }
            else if (nums[mid] < target)
            {
                l = mid + 1;
            }
            else
            {
                r = mid;
            }
        }

        return -1;
    }
};
// @lc code=end
