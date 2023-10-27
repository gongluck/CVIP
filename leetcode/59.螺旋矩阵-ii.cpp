/*
 * @lc app=leetcode.cn id=59 lang=cpp
 *
 * [59] 螺旋矩阵 II
 */

// @lc code=start
class Solution
{
public:
    vector<vector<int>> generateMatrix(int n)
    {
        std::vector<std::vector<int>> result(n, std::vector<int>(n));
        int count = 1;
        int step = 0;
        int col;
        int row;
        while (step < n / 2)
        {
            for (row = step, col = step; col < n - step - 1; ++col)
            {
                result[row][col] = count++;
            }
            for (; row < n - step - 1; ++row)
            {
                result[row][col] = count++;
            }
            for (; col > step; --col)
            {
                result[row][col] = count++;
            }
            for (; row > step; --row)
            {
                result[row][col] = count++;
            }
            ++step;
        }
        if (n % 2 == 1)
        {
            result[step][step] = count++;
        }

        return result;
    }
};
// @lc code=end
