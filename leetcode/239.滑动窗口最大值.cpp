/*
 * @lc app=leetcode.cn id=239 lang=cpp
 *
 * [239] 滑动窗口最大值
 */

// @lc code=start
class Solution
{
public:
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        int n = nums.size();
        std::vector<int> result(n - k + 1);
        std::deque<int> dq;

        for (int i = 0; i < n; ++i)
        {
            // 单调递减队列
            while (!dq.empty() && nums[dq.back()] <= nums[i])
            {
                dq.pop_back();
            }
            dq.push_back(i);

            // 确保首元素没有超出窗口范围
            if (dq.front() <= i - k)
            {
                dq.pop_front();
            }

            // 单调递减队列的首元素为当前窗口的最大值
            if (i >= k - 1)
            {
                result[i - k + 1] = nums[dq.front()];
            }
        }

        return result;
    }
};
// @lc code=end
