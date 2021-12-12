/*
 * @lc app=leetcode.cn id=209 lang=cpp
 *
 * [209] 长度最小的子数组
 */

// @lc code=start
class Solution
{
public:
    int minSubArrayLen(int target, vector<int> &nums)
    {
        // int min = INT_MAX;
        // for (int i = 0; i < nums.size(); ++i)
        // {
        //     int sum = 0;
        //     for (int j = i; j < nums.size(); ++j)
        //     {
        //         sum += nums[j];
        //         if (sum >= target)
        //         {
        //             min = std::min(min, j - i + 1);
        //             break;
        //         }
        //     }
        // }
        // return min == INT_MAX ? 0 : min;

        //滑动窗口
        int begin = 0;
        int end = 0;
        int sum = 0;
        int min = INT_MAX;
        for (; end < nums.size(); ++end)
        {
            sum += nums[end];
            while (sum >= target)
            {
                min = std::min(min, end - begin + 1);
                sum -= nums[begin++];
            }
        }
        return min == INT_MAX ? 0 : min;
    }
};
// @lc code=end
