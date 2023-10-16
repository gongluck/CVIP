/*
 * @lc app=leetcode.cn id=134 lang=cpp
 *
 * [134] 加油站
 */

// @lc code=start
class Solution
{
public:
    int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
    {
        int ret = -1;
        int n = gas.size();

        for (int i = 0; i < n; ++i)
        {
            int hasgas = 0;
            int step = 0;
            for (; step < n; ++step)
            {
                int index = (i + step) % n;
                hasgas += gas[index];
                if (hasgas < cost[index])
                {
                    break;
                }
                hasgas -= cost[index];
            }
            if (step == n) // 已走n步则找出答案
            {
                ret = i;
                break;
            }
            else
            {
                i += step; // 可以越过中间的步数
            }
        }

        return ret;
    }
};
// @lc code=end
