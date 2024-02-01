/*
 * @lc app=leetcode.cn id=93 lang=cpp
 *
 * [93] 复原 IP 地址
 */

// @lc code=start
class Solution
{
public:
    bool valid(const std::string &s, int start, int end)
    {
        if (end - start > 1 && s[start] == '0')
        {
            return false;
        }

        int val = std::atoi(s.substr(start, end - start).c_str());
        if (val < 0 || val > 255)
        {
            return false;
        }

        return true;
    }

    void dfs(const std::string &s, int start, int end,
             int steps, std::string &ip,
             std::vector<std::string> &result)
    {
        if (start >= end)
        {
            return;
        }
        if (steps == 3)
        {
            if (valid(s, start, end))
            {
                result.push_back(ip + s.substr(start, end));
            }
            return;
        }
        auto oldsize = ip.size();
        for (int i = 0; i < 3; ++i)
        {
            if (!valid(s, start, start + 1 + i))
            {
                break;
            }

            ip += s.substr(start, 1 + i) + ".";
            dfs(s, start + 1 + i, end, steps + 1, ip, result);
            ip.resize(oldsize);
        }
    }

    vector<string> restoreIpAddresses(string s)
    {
        std::vector<std::string> result;
        std::string ip;
        dfs(s, 0, s.size(), 0, ip, result);

        return result;
    }
};
// @lc code=end
