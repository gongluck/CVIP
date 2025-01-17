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
        // int slow = 0;
        // int fast = 0;
        // int n = nums.size();

        // for (; fast < n; ++fast)
        // {
        //     if (nums[fast] != val)
        //     {
        //         nums[slow++] = nums[fast];
        //     }
        // }

        // return slow;

        int left = 0;
        int right = nums.size() - 1;

        while (left <= right)
        {
            if (nums[left] != val)
            {
                ++left;
                continue;
            }

            else if (nums[right] == val)
            {
                --right;
                continue;
            }
            else
            {
                std::swap(nums[left++], nums[right]);
            }
        }

        return left;
    }
};
// @lc code=end
