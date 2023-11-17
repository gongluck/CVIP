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
        int cover = 0;

        for (int i = 0; i <= cover; ++i)
        {
            if (cover >= nums.size() - 1)
            {
                return true;
            }

            cover = std::max(cover, i + nums[i]);
        }

        return false;
    }
};
// @lc code=end
