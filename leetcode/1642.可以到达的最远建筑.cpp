/*
 * @lc app=leetcode.cn id=1642 lang=cpp
 *
 * [1642] 可以到达的最远建筑
 */

// @lc code=start
class Solution
{
public:
    int furthestBuilding(vector<int> &heights, int bricks, int ladders)
    {
        if (heights.size() <= 0)
        {
            return -1;
        }

        std::priority_queue<int> p;
        int sum = 0;

        int last = heights[0];
        int i = 1;
        for (; i < heights.size(); ++i)
        {
            auto delta = heights[i] - last;
            last = heights[i];
            if (delta <= 0)
            {
                continue;
            }

            p.push(delta);
            sum += delta;
            if (sum > bricks)
            {
                if (ladders > 0)
                {
                    --ladders;
                    sum -= p.top();
                    p.pop();
                }
                else
                {
                    break;
                }
            }
        }
        return i - 1;
    }
};
// @lc code=end
