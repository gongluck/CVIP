/*
 * @lc app=leetcode.cn id=283 lang=cpp
 *
 * [283] 移动零
 */

// @lc code=start
class Solution
{
public:
    void moveZeroes(vector<int> &nums)
    {
        //没有保持相对顺序
        // int left = 0;
        // int right = nums.size() - 1;
        // while (left < right)
        // {
        //     if (nums[right] == 0)
        //     {
        //         --right;
        //         continue;
        //     }

        //     if (nums[left] == 0)
        //     {
        //         std::swap(nums[left], nums[right]);
        //         ++left;
        //         --right;
        //     }
        //     else
        //     {
        //         ++left;
        //     }
        // }

        int fast = 0;
        int slow = 0;
        while (fast < nums.size())
        {
            if (nums[fast] != 0)
            {
                nums[slow++] = nums[fast++];
            }
            else
            {
                ++fast;
            }
        }
        while (slow < nums.size())
        {
            nums[slow++] = 0;
        }
    }
};
// @lc code=end
