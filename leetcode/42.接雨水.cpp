/*
 * @lc app=leetcode.cn id=42 lang=cpp
 *
 * [42] 接雨水
 */

// @lc code=start
class Solution
{
public:
    int trap(vector<int> &height)
    {
        int n = height.size();
        int sums = 0;
        std::stack<int> st;

        for (int i = 0; i < n; ++i)
        {
            while (!st.empty() && height[i] > height[st.top()]) // 有凹陷
            {
                int bot = st.top(); // 底部
                st.pop();
                if (!st.empty())
                {
                    int l = st.top();                                     // 左边
                    int r = i;                                            // 右边
                    int w = r - l - 1;                                    // 凹陷宽
                    int h = std::min(height[l], height[r]) - height[bot]; // 凹陷高
                    sums += w * h;
                }
            }
            st.push(i);
        }
        return sums;
    }
};
// @lc code=end
