// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem704.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=704 lang=cpp
 *
 * [704] 二分查找
 */

// @lc code=start
class Solution
{
public:
    int search(vector<int> &nums, int target)
    {
        //[left, right]
        /*
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right)
        {
            int mid = left + ((right - left) >> 1);
            int cur = nums[mid];
            if (cur == target)
            {
                return mid;
            }
            else if (cur < target)
            {
                left = mid + 1;
                continue;
            }
            else
            {
                right = mid - 1;
                continue;
            }
        }
        return -1;
        */

        //[left,right)
        int left = 0;
        int right = nums.size();
        while (left < right)
        {
            int mid = left + ((right - left) >> 2);
            int cur = nums[mid];
            if (cur == target)
            {
                return mid;
            }
            else if (cur < target)
            {
                left = mid + 1;
                continue;
            }
            else
            {
                right = mid;
                continue;
            }
        }
        return -1;
    }
};
// @lc code=end
