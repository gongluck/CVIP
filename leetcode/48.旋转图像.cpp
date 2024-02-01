/*
 * @lc app=leetcode.cn id=48 lang=cpp
 *
 * [48] 旋转图像
 */

// @lc code=start
class Solution
{
public:
    void rotate(vector<vector<int>> &matrix)
    {
        int n = matrix.size();

        for (int i = 0; i < n / 2; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                swap(matrix[i][j], matrix[n - 1 - i][j]);
            }
        }

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
};
// @lc code=end
