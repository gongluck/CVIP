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
        //static char chars[] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};
        //static int nums[] = {1, 5, 10, 50, 100, 500, 1000};
        std::map<char, int> cnums{
            {'I', 1},
            {'V', 5},
            {'X', 10},
            {'L', 50},
            {'C', 100},
            {'D', 500},
            {'M', 1000},
        };

        int last = 0;
        int curr = 0;
        int result = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            curr = cnums[s[i]];

            if (last == 0)
            {
                last = curr;
                continue;
            }

            if (last >= curr)
            {
                result += last;
                last = curr;
                continue;
            }
            else
            {
                result += curr - last;
                last = 0;
                continue;
            }
        }

        return result + last;
    }
};
// @lc code=end
