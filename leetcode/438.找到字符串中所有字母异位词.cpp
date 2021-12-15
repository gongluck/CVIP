// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem438.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=438 lang=cpp
 *
 * [438] 找到字符串中所有字母异位词
 */

// @lc code=start
#include <map>
class Solution
{
public:
    vector<int> findAnagrams(string s, string p)
    {
        std::unordered_map<char, int> need, window;
        int len = 0;
        for (const auto &c : p)
        {
            ++len;
            ++need[c];
        }

        std::vector<int> res;
        int matchs = 0;
        for (int left = 0, right = 0; right < s.size(); ++right)
        {
            if (need.count(s[right]) > 0 && ++window[s[right]] == need[s[right]])
            {
                ++matchs;
            }

            while (matchs == need.size())
            {
                if (len == right - left + 1)
                {
                    res.push_back(left);
                }

                if (need.count(s[left]) > 0 && window[s[left]]-- == need[s[left]])
                {
                    --matchs;
                }
                ++left;
            }
        }

        return res;
    }
};
// @lc code=end
