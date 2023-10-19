/*
 * @lc app=leetcode.cn id=11 lang=cpp
 *
 * [11] 盛最多水的容器
 */

// @lc code=start
class Solution
{
public:
    int maxArea(vector<int> &height)
    {
        int result = INT_MIN;
        int left = 0;
        int right = height.size() - 1;

        while (left < right)
        {
            if (height[left] <= height[right])
            {
                result = std::max(result, height[left] * (right - left));
                ++left;
            }
            else
            {
                result = std::max(result, height[right] * (right - left));
                --right;
            }
        }

        return result;
    }
};
// @lc code=end
