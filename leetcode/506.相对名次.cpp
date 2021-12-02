/*
 * @lc app=leetcode.cn id=506 lang=cpp
 *
 * [506] 相对名次
 */

// @lc code=start
class Solution
{
public:
    vector<string> findRelativeRanks(vector<int> &score)
    {
        auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b)
        {
            return a.first < b.first;
        };
        std::priority_queue<std::pair<int, int>,
                            std::vector<std::pair<int, int>>, decltype(cmp)>
            p(cmp);

        for (int i = 0; i < score.size(); ++i)
        {
            p.push(std::pair<int, int>(score[i], i));
        }

        vector<string> res(score.size(), "");
        int i = 0;
        while (!p.empty())
        {
            auto cur = p.top();
            p.pop();
            auto pos = cur.second;
            switch (i++)
            {
            case 0:
                res[pos] = "Gold Medal";
                break;
            case 1:
                res[pos] = "Silver Medal";
                break;
            case 2:
                res[pos] = "Bronze Medal";
                break;
            default:
                res[pos] = std::to_string(i);
                break;
            }
        }
        return res;
    }
};
// @lc code=end
