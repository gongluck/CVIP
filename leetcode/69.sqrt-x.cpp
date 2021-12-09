/*
 * @lc app=leetcode.cn id=69 lang=cpp
 *
 * [69] Sqrt(x)
 */

// @lc code=start
class Solution
{
public:
    int mySqrt(int x)
    {
        int left = 0;
        int right = x;

        while (left <= right)
        {
            long long mid = left + ((right - left) >> 1);
            long long cur = mid * mid;
            if (cur == x)
            {
                return mid;
            }
            else if (cur > x)
            {
                right = mid - 1;
                continue;
            }
            else
            {
                left = mid + 1;
                continue;
            }
        }

        std::cout << "left:" << left << std::endl;
        std::cout << "right:" << right << std::endl;
        return left < right ? left : right;
    }
};
// @lc code=end
