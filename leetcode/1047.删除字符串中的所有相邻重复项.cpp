/*
 * @lc app=leetcode.cn id=1047 lang=cpp
 *
 * [1047] 删除字符串中的所有相邻重复项
 */

// @lc code=start
class Solution
{
public:
    string removeDuplicates(string s)
    {
        int n = s.size();
        int slow = 0;
        int fast = 0;

        while (fast < n)
        {
            if (slow > 0 && s[slow - 1] == s[fast])
            {
                --slow;
                ++fast;
            }
            else
            {
                s[slow++] = s[fast++];
            }
        }

        s.resize(slow);
        return s;
    }
};
// @lc code=end
