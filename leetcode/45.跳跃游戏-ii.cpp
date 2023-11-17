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
        int steps = 0;
        int cover = 0;

        for (int start = 0; start <= cover; ++steps)
        {
            if (cover >= nums.size() - 1)
            {
                break;
            }

            int maxcover = cover;
            for (int i = start; i <= cover; ++i)
            {
                maxcover = std::max(maxcover, i + nums[i]);
            }
            start = cover + 1;
            cover = maxcover;
        }

        return steps;
    }
};
// @lc code=end
