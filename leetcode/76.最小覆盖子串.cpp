// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem76.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=76 lang=cpp
 *
 * [76] 最小覆盖子串
 */

// @lc code=start
class Solution
{
public:
    string minWindow(string s, string t)
    {
        std::unordered_map<char, int> need, window;
        for (const auto &c : t)
        {
            ++need[c];
        }

        int left = 0;
        int right = 0;
        int matchs = 0;
        int rstart = 0;
        int rlen = INT_MAX;
        for (; right < s.size(); ++right)
        {
            //need.count(s[right]) <= 0 时不需要加入窗口，相当于直接continue
            //need.count(s[right]) <= 0 时 matchs == need.size() 必为 false
            if (need.count(s[right]) > 0 && ++window[s[right]] == need[s[right]])
            {
                //边缘触发
                ++matchs;
            }

            bool updata = false;

            //need.count(s[right]) <= 0 时 matchs == need.size() 必为 false
            while (matchs == need.size())
            {
                //结果需要更新
                updata = true;
                if (need.count(s[left]) > 0 && window[s[left]]-- == need[s[left]])
                {
                    //边缘触发
                    --matchs;
                }

                ++left;
            }

            if (updata)
            {
                auto len = right - (left - 1) + 1;
                if (len < rlen)
                {
                    rlen = len;
                    rstart = left - 1;
                }
            }
        }

        return rlen == INT_MAX ? "" : s.substr(rstart, rlen);
    }
};
// @lc code=end
