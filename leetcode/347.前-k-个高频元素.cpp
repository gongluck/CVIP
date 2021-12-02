/*
 * @lc app=leetcode.cn id=347 lang=cpp
 *
 * [347] 前 K 个高频元素
 */

// @lc code=start
class Solution
{
public:
    vector<int> topKFrequent(vector<int> &A, int k)
    {
        unordered_map<int, int> H;
        for (auto x : A)
        {
            H[x]++;
        }

        auto cmp = [&](const pair<int, int> &a, const pair<int, int> &b)
        {
            if (a.second != b.second)
            {
                return b.second < a.second;
            }
            return b.first < a.first;
        };

        // 要出现次数高频的k个，那么要用小堆
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> Q(
            cmp);

        for (auto &p : H)
        {
            Q.push({p.first, p.second});
            while (Q.size() > k)
            {
                Q.pop();
            }
        }

        vector<int> ans;
        while (!Q.empty())
        {
            ans.push_back(Q.top().first);
            Q.pop();
        }

        sort(ans.begin(), ans.end());

        return ans;
    }
};
// @lc code=end
