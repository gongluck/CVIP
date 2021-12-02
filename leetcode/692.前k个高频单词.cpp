/*
 * @lc app=leetcode.cn id=692 lang=cpp
 *
 * [692] 前K个高频单词
 */

// @lc code=start
class Solution
{
public:
    vector<string> topKFrequent(vector<string> &words, int k)
    {
        std::unordered_map<std::string, int> h;
        for (const auto &i : words)
        {
            ++h[i];
        }

        typedef std::pair<std::string, int> Node;
        auto cmp = [](const Node &a, const Node &b)
        {
            if (a.second == b.second)
            {
                return a.first < b.first;
            }
            else
            {
                return a.second > b.second;
            }
        };
        std::priority_queue<Node, std::vector<Node>, decltype(cmp)> p(cmp);

        for (const auto &i : h)
        {
            p.push(i);
            if (p.size() > k)
            {
                p.pop();
            }
        }

        std::vector<std::string> res;
        while (!p.empty())
        {
            res.push_back(p.top().first);
            p.pop();
        }
        reverse(res.begin(), res.end());

        return res;
    }
};
// @lc code=end
