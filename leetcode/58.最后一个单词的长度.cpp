/*
 * @lc app=leetcode.cn id=58 lang=cpp
 *
 * [58] 最后一个单词的长度
 */

// @lc code=start
class Solution
{
public:
    int lengthOfLastWord(string s)
    {
        int n = s.size();
        int index = n - 1;
        int length = 0;

        while (index >= 0 && s[index] == ' ')
        {
            --index;
        }

        while (index >= 0 && s[index] != ' ')
        {
            ++length;
            --index;
        }

        return length;
    }
};
// @lc code=end
