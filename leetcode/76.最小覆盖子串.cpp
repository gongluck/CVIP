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
        for (const auto i : t)
        {
            ++need[i];
        }

        std::string res;
        int count = 0;
        for (int right = 0, left = 0; right < s.size(); ++right)
        {
            //右节点放入窗口
            window[s[right]]++;
            //判断右节点是否需要的节点
            if (window[s[right]] <= need[s[right]])
            {
                //增加符合条件的节点数
                ++count;
            }

            //判断是否要收缩窗口左边界
            while (window[s[left]] > need[s[left]])
            {
                //收缩窗口左边界
                --window[s[left++]];
            }

            //节点数匹配条件
            if (count == t.size())
            {
                if (res.empty() || right - left + 1 < res.size())
                {
                    res = s.substr(left, right - left + 1);
                }
            }
        }
        return res;
    }
};
// @lc code=end
