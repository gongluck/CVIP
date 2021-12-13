/*
 * @lc app=leetcode.cn id=54 lang=cpp
 *
 * [54] 螺旋矩阵
 */

// @lc code=start
class Solution
{
public:
    vector<int> spiralOrder(vector<vector<int>> &matrix)
    {
        int row = matrix.size();
        int col = matrix[0].size();
        vector<int> res(row * col, 0);
        int step = 0;
        int start = 0;
        for (; (start < row / 2) && (start < col / 2); ++start)
        {
            int x = start;
            int y = start;

            for (; y < col - start - 1; ++y)
            {
                res[step++] = matrix[x][y];
            }

            for (; x < row - start - 1; ++x)
            {
                res[step++] = matrix[x][y];
            }

            for (; y > start && x > start; --y)
            {
                res[step++] = matrix[x][y];
            }

            for (; x > start && y == start; --x)
            {
                res[step++] = matrix[x][y];
            }
        }

        int x = start;
        int y = start;
        while (step <= row * col - 1)
        {
            if (row < col)
            {
                res[step++] = matrix[x][y++];
            }
            else
            {
                res[step++] = matrix[x++][y];
            }
        }

        return res;
    }
};
// @lc code=end
