/*
 * @lc app=leetcode.cn id=1252 lang=cpp
 *
 * [1252] 奇数值单元格的数目
 */

// @lc code=start
class Solution
{
public:
    int oddCells(int m, int n, vector<vector<int>> &indices)
    {
        // std::vector<int> nodes(m * n, 0);
        // for (const auto &i : indices)
        // {
        //     for (int j = 0; j < n; ++j)
        //     {
        //         ++nodes[i[0] * n + j];
        //     }

        //     for (int j = 0; j < m; ++j)
        //     {
        //         ++nodes[n * j + i[1]];
        //     }
        // }

        // int res = 0;
        // for (const auto &i : nodes)
        // {
        //     //std::cout << i << std::endl;
        //     if (i % 2 == 1)
        //     {
        //         ++res;
        //     }
        // }

        // return res;

        std::vector<int> rows(m, 0);
        std::vector<int> cols(n, 0);
        for (const auto &i : indices)
        {
            ++rows[i[0]];
            ++cols[i[1]];
        }
        int res = 0;
        for (const auto &row : rows)
        {
            for (const auto &col : cols)
            {
                if (((row + col) % 2) == 1)
                {
                    ++res;
                }
            }
        }
        return res;
    }
};
// @lc code=end
