/*
 * @lc app=leetcode.cn id=165 lang=cpp
 *
 * [165] 比较版本号
 */

// @lc code=start
class Solution
{
public:
    int compareVersion(string version1, string version2)
    {
        int len1 = version1.size();
        int len2 = version2.size();
        int i = 0, j = 0;
        while (i < len1 || j < len2)
        {
            unsigned int ver1 = 0;
            while (i < len1 && version1[i] != '.')
            {
                ver1 = ver1 * 10 + version1[i] - '0';
                ++i;
            }
            ++i;

            unsigned ver2 = 0;
            while (j < len2 && version2[j] != '.')
            {
                ver2 = ver2 * 10 + version2[j] - '0';
                ++j;
            }
            ++j;

            if (ver1 != ver2)
            {
                return ver1 > ver2 ? 1 : -1;
            }
        }
        return 0;
    }
};
// @lc code=end
