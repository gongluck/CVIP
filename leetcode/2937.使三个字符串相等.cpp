/*
 * @lc app=leetcode.cn id=2937 lang=cpp
 *
 * [2937] 使三个字符串相等
 */

// @lc code=start
class Solution
{
public:
    int findMinimumOperations(string s1, string s2, string s3)
    {
        int len1 = s1.size();
        int len2 = s2.size();
        int len3 = s3.size();

        int minlen = std::min(len1, std::min(len2, len3));
        int result = 0;

        for (; result < minlen && s1[result] == s2[result] && s2[result] == s3[result]; ++result)
        {
        }

        return result == 0 ? -1 : len1 + len2 + len3 - 3 * result;
    }
};
// @lc code=end
