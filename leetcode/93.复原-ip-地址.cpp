/*
 * @lc app=leetcode.cn id=93 lang=cpp
 *
 * [93] 复原 IP 地址
 */

// @lc code=start
class Solution
{
public:
    bool checknum(const std::string &s, int start, int end)
    {
        if (end - start > 2 || start > end)
        {
            return false;
        }
        if (start != end && s[start] == '0')
        {
            return false;
        }

        auto n = std::stoi(std::string(s.c_str() + start, end - start + 1));
        if (n < 0 || n > 255)
        {
            return false;
        }

        return true;
    }

    void ipaddr(std::vector<std::string> &result, std::vector<std::string> &ip, const std::string &s, int index)
    {
        if (ip.size() == 3)
        {
            if (checknum(s, index, s.size() - 1))
            {
                result.emplace_back(ip[0] + "." + ip[1] + "." + ip[2] + "." + s.substr(index, s.size() - index));
            }
            return;
        }

        for (int i = index; i < index + 3 && i < s.size(); ++i)
        {
            if (!checknum(s, index, i))
            {
                break;
            }
            if (s.size() - i - 1 > 3 * (3 - ip.size()))
            {
                continue;
            }

            ip.push_back(s.substr(index, i - index + 1));
            ipaddr(result, ip, s, i + 1);
            ip.pop_back();
        }
    }

    vector<string> restoreIpAddresses(string s)
    {
        std::vector<std::string> result;
        std::vector<std::string> ip;

        ipaddr(result, ip, s, 0);

        return result;
    }
};
// @lc code=end
