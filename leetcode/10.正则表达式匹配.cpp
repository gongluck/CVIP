/*
 * @lc app=leetcode.cn id=10 lang=cpp
 *
 * [10] 正则表达式匹配
 */

// @lc code=start
class Solution
{
public:
    std::unordered_map<std::string, bool> visited;
    bool dp(const std::string &s, int sp,
            const std::string &p, int pp)
    {
        if (pp == p.size())
        {
            return sp == s.size();
        }

        if (sp == s.size())
        {
            if ((p.size() - pp) % 2 == 1)
            {
                return false;
            }
            for (; pp + 1 < p.size(); pp += 2)
            {
                if (p[pp + 1] != '*')
                {
                    return false;
                }
            }

            return true;
        }

        std::string key = std::to_string(sp) + "," + std::to_string(pp);
        if (visited.count(key) != 0)
        {
            return visited[key];
        }

        if (s[sp] == p[pp] || p[pp] == '.')
        {
            if (pp < p.size() - 1 && p[pp + 1] == '*')
            {
                visited[key] = (dp(s, sp, p, pp + 2) || dp(s, sp + 1, p, pp));
            }
            else
            {
                visited[key] = dp(s, sp + 1, p, pp + 1);
            }
        }
        else
        {
            if (pp < p.size() - 1 && p[pp + 1] == '*')
            {
                visited[key] = dp(s, sp, p, pp + 2);
            }
            else
            {
                visited[key] = false;
            }
        }

        return visited[key];
    }
    bool isMatch(string s, string p)
    {
        return dp(s, 0, p, 0);
    }
};
// @lc code=end
