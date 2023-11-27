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
        int result = -1;
        int n = gas.size();

        for (int i = 0; i < n; ++i)
        {
            int left = 0;
            int step = 0;

            for (step = 0; step < n; ++step)
            {
                left += gas[(i + step) % n] - cost[(i + step) % n];
                if (left < 0)
                {
                    break;
                }
            }

            if (step == n)
            {
                return i;
            }
            else
            {
                i += step;
            }
        }

        return result;
    }
};
// @lc code=end
