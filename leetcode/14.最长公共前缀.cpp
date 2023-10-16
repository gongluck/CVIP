/*
 * @lc app=leetcode.cn id=14 lang=cpp
 *
 * [14] 最长公共前缀
 */

// @lc code=start
class Solution
{
public:
    string longestCommonPrefix(vector<string> &strs)
    {
        std::string result;
        int n = strs.size();
        if (n <= 0)
        {
            return result;
        }
        result = strs[0];
        for (int i = 1; i < n; ++i)
        {
            int comparelen = std::min(result.size(), strs[i].size());
            result.resize(comparelen);
            for (int j = 0; j < comparelen; ++j)
            {
                if (result[j] != strs[i][j])
                {
                    result = result.substr(0, j);
                    break;
                }
            }
        }

        return result;
    }
};
// @lc code=end
