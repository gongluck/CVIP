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
        std::map<char, int> need, window;
        for (const auto &i : p)
        {
            ++need[i];
        }

        int left = 0;
        int right = 0;
        int count = 0;
        std::vector<int> res;
        while (right < s.size())
        {
            ++window[s[right]];
            if (window[s[right]] <= need[s[right]])
            {
                ++count;
            }

            while (window[s[left]] > need[s[left]])
            {
                --window[s[left++]];
            }

            if (count == p.size() && (right + 1 - left) == p.size())
            {
                res.push_back(left);
            }

            ++right;
        }

        return res;
    }
};
// @lc code=end
