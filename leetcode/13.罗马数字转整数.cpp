// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <map>
#include "commoncppproblem13.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=13 lang=cpp
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
class Solution
{
public:
    int romanToInt(string s)
    {
        std::map<char, int> cnums{
            {'I', 1},
            {'V', 5},
            {'X', 10},
            {'L', 50},
            {'C', 100},
            {'D', 500},
            {'M', 1000},
        };

        int sums = 0;
        int pre = 0;
        int n = s.size();
        for (int i = 0; i < n; ++i)
        {
            int val = cnums[s[i]];
            sums += val;
            if (pre < val)
            {
                sums -= 2 * pre;
            }
            pre = val;
        }

        return sums;
    }
};
// @lc code=end
