// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem51.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=51 lang=cpp
 *
 * [51] N 皇后
 */

// @lc code=start
class Solution
{
public:
    std::vector<std::vector<std::string>> res;
    bool canuse(const std::vector<std::string> &board, int row, int col)
    {
        int n = board.size();
        //行冲突不用检查
        
        //检查列冲突
        for (int i = 0; i < row; ++i)
        {
            if (board[i][col] == 'Q')
            {
                return false;
            }
        }

        //左上角
        for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j)
        {
            if (board[i][j] == 'Q')
            {
                return false;
            }
        }

        //右上角
        for (int i = row - 1, j = col + 1; i >= 0 && j < n; --i, ++j)
        {
            if (board[i][j] == 'Q')
            {
                return false;
            }
        }

        //左下角不用检查
        //右下角不用检查

        return true;
    }
    void dfs(std::vector<std::string> &board, int row)
    {
        //深度遍历结束
        if (row >= board.size())
        {
            //增加决策结果
            res.push_back(board);
            return;
        }

        //遍历候选决策
        int n = board[row].size();
        for (int i = 0; i < n; ++i)
        {
            //检查候选项
            if (canuse(board, row, i))
            {
                //决策
                board[row][i] = 'Q';
                //深度递进决策
                dfs(board, row + 1);
                //撤销决策
                board[row][i] = '.';
            }
        }
    }
    vector<vector<string>> solveNQueens(int n)
    {
        //空棋盘
        std::vector<std::string> board(n, std::string(n, '.'));
        //逐行深度遍历
        dfs(board, 0);
        return res;
    }
};
// @lc code=end
