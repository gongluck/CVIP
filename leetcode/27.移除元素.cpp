/*
 * @lc app=leetcode.cn id=27 lang=cpp
 *
 * [27] 移除元素
 */

// @lc code=start
class Solution
{
public:
    int removeElement(vector<int> &nums, int val)
    {
        // 双向指针
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right)
        {
            if (nums[left] != val)
            {
                ++left;
            }
            // else if(nums[right] == val)
            // {
            //     --right;
            // }
            else 
            {
                nums[left] = nums[right--];
            }
        }

        return left;
    }
};
// @lc code=end
