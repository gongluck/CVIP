/*
 * @lc app=leetcode.cn id=198 lang=cpp
 *
 * [198] 打家劫舍
 */

// @lc code=start
class Solution
{
public:
    int rob(vector<int> &nums)
    {
        // pre2 pre1 cur
        int pre2 = 0;
        int pre1 = nums[0];
        int cur = nums[0];

        for (int i = 1; i < nums.size(); ++i)
        {
            cur = std::max(pre2 + nums[i], pre1);
            pre2 = pre1;
            pre1 = cur;
        }

        return cur;
    }
};
// @lc code=end
