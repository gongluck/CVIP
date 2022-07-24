/*
 * @lc app=leetcode.cn id=1217 lang=cpp
 *
 * [1217] 玩筹码
 */

// @lc code=start
class Solution
{
public:
    int minCostToMoveChips(vector<int> &position)
    {
        // int res = INT_MAX;
        // int len = position.size();
        // std::map<int, int> result;
        // for (int i = 0; i < len; ++i)
        // {
        //     if (result.count(position[i]) > 0)
        //     {
        //         continue;
        //     }

        //     int cur = 0;
        //     for (int j = 0; j < len; ++j)
        //     {
        //         if (i == j || position[i] == position[j])
        //         {
        //             continue;
        //         }
        //         int diff = abs(position[i] - position[j]);
        //         //std::cout << i << j <<  diff << std::endl;
        //         cur += diff % 2;
        //     }

        //     res = min(res, cur);
        //     //std::cout << "res : " << res << std::endl;
        //     result[position[i]] = res;
        // }

        // return res;

        //奇偶移动统计
        int ji = 0, ou = 0;
        for (const auto &i : position)
        {
            if (i % 2 == 0)
            {
                ++ou;
            }
            else
            {
                ++ji;
            }
        }
        return min(ji, ou);
    }
};
// @lc code=end
