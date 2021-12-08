/*
 * @lc app=leetcode.cn id=35 lang=cpp
 *
 * [35] 搜索插入位置
 */

// @lc code=start
class Solution
{
public:
    int searchInsert(vector<int> &nums, int target)
    {
        //暴力解法
        // for (int i = 0; i < nums.size(); ++i)
        // {
        //     if (nums[i] >= target)
        //     {
        //         return i;
        //     }
        // }
        // return nums.size();

        //二分查找1
        // int left = 0;
        // int right = nums.size() - 1;
        // while (left <= right)
        // {
        //     int mid = left + ((right - left) >> 1);
        //     int cur = nums[mid];
        //     if (cur == target)
        //     {
        //         return mid;
        //     }
        //     else if (cur < target)
        //     {
        //         left = mid + 1;
        //         continue;
        //     }
        //     else
        //     {
        //         right = mid - 1;
        //         continue;
        //     }
        // }
        // return right + 1;

        //二分查找2
        int left = 0;
        int right = nums.size();
        while (left < right)
        {
            int mid = left + ((right - left) >> 1);
            int cur = nums[mid];
            if (cur == target)
            {
                return mid;
            }
            else if (cur < target)
            {
                left = mid + 1;
                continue;
            }
            else
            {
                right = mid;
                continue;
            }
        }
        return right;
    }
};
// @lc code=end
