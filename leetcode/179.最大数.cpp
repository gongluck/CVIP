/*
 * @lc app=leetcode.cn id=179 lang=cpp
 *
 * [179] 最大数
 */

// @lc code=start
class Solution
{
public:
    string largestNumber(vector<int> &nums)
    {
        std::vector<std::string> strs;

        for (const auto &n : nums)
        {
            strs.push_back(std::to_string(n));
        }
        std::sort(strs.begin(), strs.end(),
                  [](const std::string &s1, const std::string &s2)
                  { return s1 + s2 > s2 + s1; });

        std::string result;
        for (const auto &s : strs)
        {
            result.append(s);
        }
        if (result[0] == '0')
        {
            result = "0";
        }
        return result;
    }
};
// @lc code=end
