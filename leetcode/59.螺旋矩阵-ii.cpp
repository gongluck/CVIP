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
        vector<vector<int>> res(n, vector<int>(n, 0));
        int startx = 0;
        int starty = 0;
        int track = 1;
        while (startx < n / 2)
        {
            int x = startx;
            int y = starty;

            for (; y < n - starty - 1; ++y)
            {
                res[x][y] = track++;
            }

            for (; x < n - startx - 1; ++x)
            {
                res[x][y] = track++;
            }

            //std::cout << "x : " << x << ", y : " << y << std::endl;

            for (; y > starty; --y)
            {
                res[x][y] = track++;
            }

            for (; x > startx; --x)
            {
                res[x][y] = track++;
            }

            ++startx;
            ++starty;
        }

        if (n % 2 == 1)
        {
            res[startx][starty] = track;
        }

        return res;
    }
};
// @lc code=end
