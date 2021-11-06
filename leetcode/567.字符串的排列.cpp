// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem567.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=567 lang=cpp
 *
 * [567] 字符串的排列
 */

// @lc code=start
class Solution
{
public:
    bool checkInclusion(string s1, string s2)
    {
        std::unordered_map<char, int> need, window;
        for (const auto &i : s1)
        {
            ++need[i];
        }

        int left = 0;
        int right = 0;
        int count = 0;
        while (right < s2.size())
        {
            ++window[s2[right]];
            if (window[s2[right]] <= need[s2[right]])
            {
                ++count;
            }

            while(window[s2[left]] > need[s2[left]])
            {
                --window[s2[left++]];
            }

            if (count == s1.size() && (right + 1 - left) == s1.size())
            {
                return true;
            }

            ++right;
        }

        return false;
    }
};
// @lc code=end
