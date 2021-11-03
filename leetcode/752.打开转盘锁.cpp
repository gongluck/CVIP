// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem752.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=752 lang=cpp
 *
 * [752] 打开转盘锁
 */

// @lc code=start
class Solution
{
public:
    std::string up(const std::string &str, int pos)
    {
        std::string restr = str;
        switch (str[pos])
        {
        case '9':
            restr[pos] = '0';
            break;
        default:
            restr[pos] += 1;
            break;
        }
        return restr;
    }
    std::string down(const std::string &str, int pos)
    {
        std::string restr = str;
        switch (str[pos])
        {
        case '0':
            restr[pos] = '9';
            break;
        default:
            restr[pos] -= 1;
            break;
        }
        return restr;
    }
    int openLock(vector<string> &deadends, string target)
    {
        std::queue<std::string> q;
        std::unordered_set<std::string> visited(deadends.begin(), deadends.end());

        int steps = 0;
        if (visited.find("0000") != visited.end())
        {
            return -1;
        }
        q.push("0000");
        visited.insert("0000");

        while (!q.empty())
        {
            auto sz = q.size();
            for (int i = 0; i < sz; ++i)
            {
                auto cur = q.front();
                q.pop();

                if (cur == target)
                {
                    return steps;
                }

                std::string next;
                for (int i = 0; i < 4; ++i)
                {
                    next = up(cur, i);
                    if (visited.find(next) == visited.end())
                    {
                        visited.insert(next);
                        q.push(next);
                    }
                    next = down(cur, i);
                    if (visited.find(next) == visited.end())
                    {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }

            ++steps;
        }

        return -1;
    }
};
// @lc code=end
