/*
 * @lc app=leetcode.cn id=131 lang=cpp
 *
 * [131] 分割回文串
 */

// @lc code=start
class Solution
{
public:
    bool isroundback(const std::string &s, int start, int end)
    {
        while (start < end)
        {
            if (s[start++] != s[end--])
            {
                return false;
            }
        }

        return true;
    }

    void part(std::vector<std::vector<std::string>> &result, std::vector<std::string> &steps, const std::string &s, int index)
    {
        if (index >= s.size())
        {
            result.emplace_back(steps);
            return;
        }

        for (int i = index; i < s.size(); ++i)
        {
            if (isroundback(s, index, i))
            {
                // std::cout << "push: " << s.substr(index, i - index + 1) << std::endl;
                steps.push_back(s.substr(index, i - index + 1));
                part(result, steps, s, i + 1);
                steps.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s)
    {
        std::vector<std::vector<std::string>> result;
        std::vector<std::string> steps;

        part(result, steps, s, 0);

        return result;
    }
};
// @lc code=end
