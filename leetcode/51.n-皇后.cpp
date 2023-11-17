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
    bool canuse(const std::vector<std::string> &steps, int col, int n)
    {
        for (int row = 0; row < steps.size(); ++row)
        {
            if (steps[row][col] == 'Q')
            {
                return false;
            }
        }

        int row = steps.size();
        for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; r -= 1, c -= 1)
        {
            if (steps[r][c] == 'Q')
            {
                return false;
            }
        }
        for (int r = row - 1, c = col + 1; r >= 0 && c < n; r -= 1, c += 1)
        {
            if (steps[r][c] == 'Q')
            {
                return false;
            }
        }

        return true;
    }

    void solve(std::vector<std::vector<std::string>> &result, int n, std::vector<std::string> &steps)
    {
        if (steps.size() == n)
        {
            result.emplace_back(steps);
            return;
        }

        for (int i = 0; i < n; ++i)
        {
            if (canuse(steps, i, n))
            {
                std::string s(n, '.');
                s[i] = 'Q';
                steps.emplace_back(s);
                solve(result, n, steps);
                steps.pop_back();
            }
        }
    }

    vector<vector<string>> solveNQueens(int n)
    {
        std::vector<std::vector<std::string>> result;
        std::vector<std::string> steps;

        solve(result, n, steps);

        return result;
    }
};
// @lc code=end
