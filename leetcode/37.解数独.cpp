/*
 * @lc app=leetcode.cn id=37 lang=cpp
 *
 * [37] 解数独
 */

// @lc code=start
class Solution
{
public:
    bool canuse(vector<vector<char>> &board, int row, int col, int use)
    {
        for (int i = 0; i < 9; ++i)
        {
            if (board[i][col] == use)
            {
                return false;
            }
            if (board[row][i] == use)
            {
                return false;
            }
            if (board[(row / 3) * 3 + i / 3][(col / 3) * 3 + i % 3] == use)
            {
                return false;
            }
        }
        return true;
    }
    bool dp(vector<vector<char>> &board, int row, int col)
    {
        if (col == 9)
        {
            row += 1;
            col = 0;
        }
        if (row == 9)
        {
            return true;
        }
        if (board[row][col] != '.')
        {
            return dp(board, row, col + 1);
        }
        for (int i = '1'; i <= '9'; ++i)
        {
            if (canuse(board, row, col, i))
            {
                board[row][col] = i;
                if (dp(board, row, col + 1))
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
        dp(board, 0, 0);
    }
};
// @lc code=end
