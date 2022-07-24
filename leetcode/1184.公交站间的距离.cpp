/*
 * @lc app=leetcode.cn id=1184 lang=cpp
 *
 * [1184] 公交站间的距离
 */

// @lc code=start
class Solution
{
public:
    int distanceBetweenBusStops(vector<int> &distance, int start, int destination)
    {
        int ltrack = 0;
        int rtrack = 0;
        if (start > destination)
        {
            std::swap(start, destination);
        }
        for (int i = start; i < destination; ++i)
        {
            ltrack += distance[i];
        }
        for (int i = 0; i < start; ++i)
        {
            rtrack += distance[i];
        }
        for (int i = destination; i < distance.size(); ++i)
        {
            rtrack += distance[i];
        }

        return std::min(ltrack, rtrack);
    }
};
// @lc code=end
