/*
 * @lc app=leetcode.cn id=53 lang=cpp
 *
 * [53] 最大子序和
 */

// @lc code=start
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if(nums.size() == 0)
        {
            return 0;
        }

        int dp_last = nums[0];
        int dp_cur = 0;
        int res = nums[0];
        for(int i = 1; i<nums.size(); ++i)
        {
            dp_cur = std::max(dp_last + nums[i], nums[i]);
            dp_last = dp_cur;
            res = std::max(res, dp_cur);
        }

        return res;
    }
};
// @lc code=end

