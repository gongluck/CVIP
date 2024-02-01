/*
 * @lc app=leetcode.cn id=215 lang=cpp
 *
 * [215] 数组中的第K个最大元素
 */

// @lc code=start
class Solution
{
public:
    int findKthLargest(vector<int> &nums, int k)
    {
        std::priority_queue<int> priority;

        for (const auto &n : nums)
        {
            priority.push(n);
        }
        for (int i = 0; i < k - 1; ++i)
        {
            priority.pop();
        }

        return priority.top();
    }
};
// @lc code=end
