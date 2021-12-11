/*
 * @lc app=leetcode.cn id=977 lang=cpp
 *
 * [977] 有序数组的平方
 */

// @lc code=start
class Solution
{
public:
    vector<int> sortedSquares(vector<int> &nums)
    {
        vector<int> res(nums.size(), 0);
        int l = 0;
        int r = nums.size() - 1;
        int pos = nums.size() - 1;
        while (pos >= 0)
        {
            long long ll = nums[l] * nums[l];
            long long rr = nums[r] * nums[r];
            if (ll < rr)
            {
                res[pos--] = rr;
                --r;
            }
            else
            {
                res[pos--] = ll;
                ++l;
            }
        }
        return res;
    }
};
// @lc code=end
