/*
 * @lc app=leetcode.cn id=49 lang=cpp
 *
 * [49] 字母异位词分组
 */

// @lc code=start
class Solution
{
public:
    vector<vector<string>> groupAnagrams(vector<string> &strs)
    {
        unordered_map<string, vector<string>> sortstrnums;
        for (const auto &s : strs)
        {
            auto tmp = s;
            sort(tmp.begin(), tmp.end());
            sortstrnums[tmp].push_back(s);
        }
        vector<vector<string>> res;
        for (const auto &sv : sortstrnums)
        {
            res.push_back(sv.second);
        }
        return res;
    }
};
// @lc code=end
