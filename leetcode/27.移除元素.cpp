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
        //暴力解法
        // int size = nums.size();
        // for (int i = 0; i < size; ++i)
        // {
        //     if (nums[i] == val)
        //     {
        //         --size;
        //         for (int j = i; j < size; ++j)
        //         {
        //             nums[j] = nums[j + 1];
        //         }
        //         --i;
        //     }
        // }
        // return size;

        //双指针
        int fast = 0;
        int slow = 0;
        while (fast < nums.size())
        {
            if (nums[fast] != val)
            {
                nums[slow++] = nums[fast++];
            }
            else
            {
                ++fast;
            }
        }
        return slow;
    }
};
// @lc code=end
