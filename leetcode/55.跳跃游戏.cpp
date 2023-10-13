/*
 * @lc app=leetcode.cn id=55 lang=cpp
 *
 * [55] 跳跃游戏
 */

// @lc code=start
class Solution
{
public:
    bool canJump(vector<int> &nums)
    {
        int n = nums.size();
        int rightmost = 0;
        for (int i = 0; i < n; ++i)
        {
            if (i <= rightmost)
            {
                rightmost = std::max(rightmost, nums[i] + i);
                if (rightmost >= n - 1)
                {
                    return true;
                }
            }
        }
        return false;
    }
};
// @lc code=end
