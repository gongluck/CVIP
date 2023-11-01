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

        std::reverse(s.begin(), s.end());
        for (int start = 0; start < n;)
        {
            if (s[start] == ' ')
            {
                ++start;
                continue;
            }

            int end = start + 1;
            while (end < n && s[end] != ' ')
            {
                ++end;
            }

            if (end - start > 1)
            {
                std::reverse(s.begin() + start, s.begin() + end);
            }

            start = end + 1;
        }

        int fast = 0;
        int slow = 0;
        while (fast < n)
        {
            if (s[fast] != ' ' || (slow > 0 && fast < n - 1 && s[fast + 1] != ' '))
            {
                s[slow++] = s[fast];
            }

            ++fast;
        }
        s.resize(slow);

        return s;
    }
};
// @lc code=end
