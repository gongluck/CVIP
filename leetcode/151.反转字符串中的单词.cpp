/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 反转字符串中的单词
 */

// @lc code=start
class Solution
{
public:
    string reverseWords(string s)
    {
        int n = s.size();
        std::deque<std::string> d;
        std::string word;
        std::string result;

        for (int i = 0; i < n; ++i)
        {
            char c = s[i];
            if (c != ' ')
            {
                word += c;
            }
            else if (word.size() > 0)
            {
                d.push_front(word);
                word.clear();
            }
        }
        if (word.size() > 0)
        {
            d.push_front(word);
            word.clear();
        }

        while (!d.empty())
        {
            result += d.front();
            d.pop_front();
            if (!d.empty())
                result += ' ';
        }
        return result;
    }
};
// @lc code=end
