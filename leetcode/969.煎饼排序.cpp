/*
 * @lc app=leetcode.cn id=969 lang=cpp
 *
 * [969] 煎饼排序
 */

// @lc code=start
#include <algorithm>
class Solution
{
public:
    vector<int> res;
    void dfs(vector<int> &arr, int pos)
    {
        if (pos <= 1)
        {
            return;
        }

        int max = pos - 1;
        for (int i = pos - 2; i >= 0; --i)
        {
            if (arr[max] < arr[i])
            {
                max = i;
            }
        }

        if (pos > 2)
        {
            res.push_back(max + 1);
            std::reverse(arr.begin(), arr.begin() + max + 1);
            res.push_back(pos);
            std::reverse(arr.begin(), arr.begin() + pos);
        }
        else if (pos == 2 && max != 1)
        {
            res.push_back(2);
            std::reverse(arr.begin(), arr.begin() + 2);
        }

        dfs(arr, pos - 1);
    }
    vector<int> pancakeSort(vector<int> &arr)
    {
        dfs(arr, arr.size());
        return res;
    }
};
// @lc code=end
