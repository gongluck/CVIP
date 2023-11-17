/*
 * @lc app=leetcode.cn id=17 lang=cpp
 *
 * [17] 电话号码的字母组合
 */

// @lc code=start
class Solution
{
public:
    void letter(std::vector<std::string> &result, std::unordered_map<char, std::string> &maps, const std::string &digits, std::string &s, int index)
    {
        if (digits.size() == s.size())
        {
            if (s.size() != 0)
            {
                result.emplace_back(s);
            }

            return;
        }

        for (const auto &c : maps[digits[index]])
        {
            s.push_back(c);
            letter(result, maps, digits, s, index + 1);
            s.pop_back();
        }
    }

    vector<string> letterCombinations(string digits)
    {
        std::vector<std::string> result;
        std::unordered_map<char, std::string> maps;
        std::string s;

        maps['2'] = "abc";
        maps['3'] = "def";
        maps['4'] = "ghi";
        maps['5'] = "jkl";
        maps['6'] = "mno";
        maps['7'] = "pqrs";
        maps['8'] = "tuv";
        maps['9'] = "wxyz";

        letter(result, maps, digits, s, 0);

        return result;
    }
};
// @lc code=end
