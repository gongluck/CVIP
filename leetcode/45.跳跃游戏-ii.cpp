/*
 * @lc app=leetcode.cn id=45 lang=cpp
 *
 * [45] 跳跃游戏 II
 */

// @lc code=start
class Solution
{
public:
    int jump(vector<int> &nums)
    {
        int n = nums.size();
        int steps = 0;
        for (int right = n - 1; right > 0;)
        {
            for (int left = 0; left < right; ++left)
            {
                if (left + nums[left] >= right)
                {
                    ++steps;
                    right = left;
                    break;
                }
            }
        }
        return steps;
    }
};
// @lc code=end
