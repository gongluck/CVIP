/*
 * @lc app=leetcode.cn id=37 lang=cpp
 *
 * [37] 解数独
 */

// @lc code=start
class Solution
{
public:
    bool canuse(const std::vector<std::vector<char>> &board, int row, int col, int n, char c)
    {
        for (int i = 0; i < n; ++i)
        {
            if (board[i][col] == c || board[row][i] == c ||
                board[(row / 3) * 3 + i / 3][(col / 3) * 3 + i % 3] == c)
            {
                return false;
            }
        }

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (board[(row / 3) * 3 + i][(col / 3) * 3 + j] == c)
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool solve(std::vector<std::vector<char>> &board, int row, int col, int n)
    {
        if (col == n)
        {
            row += 1;
            col = 0;
        }
        if (row >= n)
        {
            return true;
        }

        if (board[row][col] != '.')
        {
            return solve(board, row, col + 1, n);
        }

        for (auto c = '1'; c <= '9'; ++c)
        {
            if (canuse(board, row, col, n, c))
            {
                board[row][col] = c;
                if (solve(board, row, col + 1, n))
                {
                    return true;
                }
                board[row][col] = '.';
            }
        }

        return false;
    }

    void solveSudoku(vector<vector<char>> &board)
    {
        solve(board, 0, 0, board.size());
    }
};
// @lc code=end
