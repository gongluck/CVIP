/*
 * @lc app=leetcode.cn id=34 lang=cpp
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 */

// @lc code=start
class Solution
{
public:
    vector<int> searchRange(vector<int> &nums, int target)
    {
        int left = 0;
        int right = nums.size() - 1;
        int rightborder = -2;
        while (left <= right)
        {
            int mid = left + ((right - left) >> 1);
            int cur = nums[mid];
            if (cur > target)
            {
                right = mid - 1;
                continue;
            }
            else if (cur <= target)
            {
                left = mid + 1;
                rightborder = left;
                continue;
            }
        }
        std::cout << "rightborder:" << rightborder << std::endl;

        left = 0;
        right = nums.size() - 1;
        int leftborder = -2;
        while (left <= right)
        {
            int mid = left + ((right - left) >> 1);
            int cur = nums[mid];
            if (cur < target)
            {
                left = mid + 1;
                continue;
            }
            else if (cur >= target)
            {
                right = mid - 1;
                leftborder = right;
                continue;
            }
        }
        std::cout << "leftborder:" << leftborder << std::endl;

        if (leftborder == -2 || rightborder == -2 || leftborder + 1 >= rightborder)
        {
            return {-1, -1};
        }
        else
        {
            return {leftborder + 1, rightborder - 1};
        }
    }
};
// @lc code=end
