/*
 * @lc app=leetcode.cn id=30 lang=cpp
 *
 * [30] 串联所有单词的子串
 */

// @lc code=start
class Solution
{
public:
    vector<int> findSubstring(string s, vector<string> &words)
    {
        std::vector<int> result;
        int N = s.size();
        int n = words[0].size();
        int left = 0;
        int right = words.size() * n;
        std::unordered_set<std::string> sets;
        for (const auto &w : words)
        {
            sets.insert(w);
        }

        while (right < N)
        {
            
        }

        return result;
    }
};
// @lc code=end
