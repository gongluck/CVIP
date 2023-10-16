/*
 * @lc app=leetcode.cn id=135 lang=cpp
 *
 * [135] 分发糖果
 */

// @lc code=start
class Solution
{
public:
    int candy(vector<int> &ratings)
    {
        int n = ratings.size();
        int sum = 0;

        std::vector<int> left(n, 1);

        for (int i = 1; i < n; ++i)
        {
            if (ratings[i] > ratings[i - 1])
            {
                left[i] = left[i - 1] + 1;
            }
        }

        int right = 0;
        for (int i = n - 1; i >= 0; --i)
        {
            if (i < n - 1 && ratings[i] > ratings[i + 1])
            {
                ++right;
            }
            else
            {
                right = 1;
            }
            sum += std::max(left[i], right);
        }

        return sum;
    }
};
// @lc code=end
