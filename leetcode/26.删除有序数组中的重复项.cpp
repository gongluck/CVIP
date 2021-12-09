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
        int fast = 0;
        int slow = 0;
        int last = INT_MAX;
        while (fast < nums.size())
        {
            int cur = nums[fast];
            if (cur == last)
            {
                ++fast;
            }
            else
            {
                nums[slow++] = nums[fast++];
                last = cur;
            }
        }
        return slow;
    }
};
// @lc code=end
