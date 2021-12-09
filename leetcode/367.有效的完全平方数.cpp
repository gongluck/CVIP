/*
 * @lc app=leetcode.cn id=367 lang=cpp
 *
 * [367] 有效的完全平方数
 */

// @lc code=start
class Solution
{
public:
    bool isPerfectSquare(int num)
    {
        int left = 0;
        int right = num;
        while (left <= right)
        {
            long long mid = left + ((right - left) >> 1);
            long long cur = mid * mid;
            if (cur == num)
            {
                return true;
            }
            else if (cur < num)
            {
                left = mid + 1;
                continue;
            }
            else
            {
                right = mid - 1;
                continue;
            }
        }
        return false;
    }
};
// @lc code=end
