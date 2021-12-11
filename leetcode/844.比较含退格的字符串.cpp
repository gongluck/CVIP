/*
 * @lc app=leetcode.cn id=844 lang=cpp
 *
 * [844] 比较含退格的字符串
 */

// @lc code=start
class Solution
{
public:
    bool backspaceCompare(string s, string t)
    {
        int i = s.size() - 1;
        int j = t.size() - 1;
        int delcount = 0;

        while (true)
        {
            delcount = 0;
            while (i >= 0)
            {
                if (s[i] == '#')
                {
                    //待删除字符数+1
                    ++delcount;
                }
                else if (delcount > 0)
                {
                    //待删除字符数-1
                    --delcount;
                }
                else //s[i]!='#' && delcount<=0
                {
                    break;
                }
                --i;
            }

            delcount = 0;
            while (j >= 0)
            {
                if (t[j] == '#')
                {
                    //待删除字符数+1
                    ++delcount;
                }
                else if (delcount > 0)
                {
                    //待删除字符数-1
                    --delcount;
                }
                else //s[i]!='#' && delcount<=0
                {
                    break;
                }
                --j;
            }

            if (i >= 0 && j >= 0)
            {
                if (s[i] != t[j])
                {
                    return false;
                }
                else
                {
                    --i;
                    --j;
                    continue;
                }
            }
            else if (i < 0 && j < 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
};
// @lc code=end
