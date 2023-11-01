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
        std::unordered_map<std::string, int> maps;
        std::priority_queue<
            std::pair<std::string, int>,
            std::vector<std::pair<std::string, int>>,
            std::function<bool(std::pair<std::string, int> & l, std::pair<std::string, int> & r)>>
        pq([](std::pair<std::string, int> &l, std::pair<std::string, int> &r) -> bool // 返回true则l沉底
           {
                if (l.second != r.second)
                {
                    return !(l.second > r.second);//大顶堆
                }
                else
                {
                    return l.first > r.first; //字典顺序 小顶堆
                } });
        std::vector<std::string> result(k);
        result.resize(0);

        for (const auto &s : words)
        {
            ++maps[s];
        }
        for (const auto &m : maps)
        {
            pq.push(m);
        }
        while (k-- > 0)
        {
            result.push_back(pq.top().first);
            pq.pop();
        }

        return result;
    }
};
// @lc code=end
