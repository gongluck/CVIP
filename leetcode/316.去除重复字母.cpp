/*
 * @lc app=leetcode.cn id=316 lang=cpp
 *
 * [316] 去除重复字母
 */

// @lc code=start
class Solution
{
public:
    string removeDuplicateLetters(string s)
    {
        int counts[27] = {0};
        bool inresult[27] = {false};
        for (const auto &c : s)
        {
            ++counts[c - 'a'];
        }

        std::string result;
        for (const auto &c : s)
        {
            --counts[c - 'a'];
            if (inresult[c - 'a'])
            {
                continue;
            }
            else
            {
                while (!result.empty() &&
                       result[result.size() - 1] > c &&
                       counts[result[result.size() - 1] - 'a'] > 0)
                {
                    inresult[result[result.size() - 1] - 'a'] = false;
                    result.pop_back();
                }
                result.push_back(c);
                inresult[c - 'a'] = true;
            }
        }

        return result;
    }
};
// @lc code=end
