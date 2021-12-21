/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 翻转字符串里的单词
 */

// @lc code=start
class Solution
{
public:
    // 反转字符串s中左闭又闭的区间[start, end]
    void reverse(string &s, int start, int end)
    {
        for (int i = start, j = end; i < j; i++, j--)
        {
            swap(s[i], s[j]);
        }
    }

    // 移除冗余空格：使用双指针（快慢指针法）O(n)的算法
    void removeExtraSpaces(string &s)
    {
        int slowIndex = 0, fastIndex = 0; // 定义快指针，慢指针
        // 去掉字符串前面的空格
        while (s.size() > 0 && fastIndex < s.size() && s[fastIndex] == ' ')
        {
            fastIndex++;
        }
        for (; fastIndex < s.size(); fastIndex++)
        {
            // 去掉字符串中间部分的冗余空格
            if (fastIndex - 1 > 0 && s[fastIndex - 1] == s[fastIndex] && s[fastIndex] == ' ')
            {
                continue;
            }
            else
            {
                s[slowIndex++] = s[fastIndex];
            }
        }
        if (slowIndex - 1 > 0 && s[slowIndex - 1] == ' ')
        { // 去掉字符串末尾的空格
            s.resize(slowIndex - 1);
        }
        else
        {
            s.resize(slowIndex); // 重新设置字符串大小
        }
    }

    string reverseWords(string s)
    {
        removeExtraSpaces(s);
        reverse(s, 0, s.size() - 1);
        for (int i = 0; i < s.size(); i++)
        {
            int j = i;
            // 查找单词间的空格，翻转单词
            while (j < s.size() && s[j] != ' ')
                j++;
            reverse(s, i, j - 1);
            i = j;
        }
        return s;
    }
};
// @lc code=end
