/*
 * @lc app=leetcode.cn id=202 lang=cpp
 *
 * [202] 快乐数
 */

// @lc code=start
class Solution
{
public:
    bool isHappy(int n)
    {
        std::unordered_set<int> visited;
        while (true)
        {
            if (visited.count(n) > 0)
            {
                return false;
            }
            int tmp = n;
            int sum = 0;
            while (tmp > 0)
            {
                int i = tmp % 10;
                tmp /= 10;
                sum += i * i;
            }
            if (sum == 1)
            {
                return true;
            }
            visited.insert(n);
            n = sum;
        }
    }
};
// @lc code=end
