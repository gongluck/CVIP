// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem904.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=904 lang=cpp
 *
 * [904] 水果成篮
 */

// @lc code=start
class Solution
{
public:
    int totalFruit(vector<int> &fruits)
    {
        int max = INT_MIN;
        int begin = 0;
        int end = 0;
        std::unordered_map<int, int> s;
        for (; end < fruits.size(); ++end)
        {
            ++s[fruits[end]];
            while (s.size() > 2)
            {
                if (--s[fruits[begin]] == 0)
                {
                    s.erase(fruits[begin]);
                }
                ++begin;
            }
            max = std::max(max, end - begin + 1);
        }
        return max;
    }
};
// @lc code=end
