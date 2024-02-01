/*
 * @lc app=leetcode.cn id=75 lang=cpp
 *
 * [75] 颜色分类
 */

// @lc code=start
class Solution
{
public:
    void sortColors(vector<int> &nums)
    {
        int len = nums.size();
        int zero = 0;
        int two = len - 1;

        for (int i = 0; i <= two; ++i)
        {
            if (nums[i] == 0)
            {
                std::swap(nums[i], nums[zero++]);
            }
            else if (nums[i] == 2)
            {
                std::swap(nums[i--], nums[two--]);
            }
        }
    }
};
// @lc code=end
