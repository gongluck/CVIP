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
        int n = nums.size();
        std::vector<int> result(n);
        int k = n - 1;
        int i = 0;
        int j = n - 1;
        int powi, powj;

        while (k >= 0)
        {
            powi = std::pow(nums[i], 2);
            powj = std::pow(nums[j], 2);
            if (powi > powj)
            {
                result[k--] = powi;
                ++i;
            }
            else
            {
                result[k--] = powj;
                --j;
            }
        }

        return result;
    }
};
// @lc code=end
