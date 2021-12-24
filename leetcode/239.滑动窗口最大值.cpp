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
        std::vector<int> res;
        std::deque<int> q;
        for (int i = 0; i < nums.size(); ++i)
        {
            while (!q.empty() && q.back() < nums[i])
            {
                q.pop_back();
            }
            q.push_back(nums[i]);

            if (i + 1 >= k)
            {
                res.push_back(q.front());
                if (nums[i - k + 1] == q.front())
                {
                    q.pop_front();
                }
            }
        }
        return res;
    }
};
// @lc code=end
