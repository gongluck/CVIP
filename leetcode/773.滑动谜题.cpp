/*
 * @lc app=leetcode.cn id=773 lang=cpp
 *
 * [773] 滑动谜题
 */

// @lc code=start
class Solution
{
public:
    int slidingPuzzle(vector<vector<int>> &board)
    {
        string target = "123450";
        set<string> visited;
        vector<vector<int>> swapos = {{1, 3}, {0, 2, 4}, {1, 5}, {0, 4}, {1, 3, 5}, {2, 4}};
        int step = 0;
        queue<string> q;
        string start = target;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                start[i * 3 + j] = (char)(board[i][j] + '0');
            }
        }
        q.push(start);

        while (!q.empty())
        {
            int sz = q.size();
            for (int i = 0; i < sz; ++i)
            {
                auto cur = q.front();
                q.pop();
                if (cur == target)
                {
                    return step;
                }
                for (int idx = 0; idx < 6; ++idx)
                {
                    if (cur[idx] == '0')
                    {
                        for (auto adj : swapos[idx])
                        {
                            auto newcur = cur;
                            std::swap(newcur[adj], newcur[idx]);
                            if (visited.count(newcur) <= 0)
                            {
                                q.push(newcur);
                                visited.insert(newcur);
                            }
                        }
                    }
                }
            }
            ++step;
        }
        return -1;
    }
};
// @lc code=end
