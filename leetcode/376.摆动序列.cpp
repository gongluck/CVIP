/*
 * @lc app=leetcode.cn id=376 lang=cpp
 *
 * [376] 摆动序列
 */

// @lc code=start
class Solution
{
public:
    int wiggleMaxLength(vector<int> &nums)
    {
        if (nums.size() <= 1)
        {
            return nums.size();
        }

        int result = 1;
        int prediff = 0;
        for (int i = 1; i < nums.size(); ++i)
        {
            int curdiff = nums[i] - nums[i - 1];
            if (curdiff * prediff < 0 || (prediff == 0 && curdiff != 0))
            {
                ++result;
                prediff = curdiff;
            }
        }

        return result;
    }
};
// @lc code=end
