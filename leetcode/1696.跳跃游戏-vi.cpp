/*
 * @lc app=leetcode.cn id=1696 lang=cpp
 *
 * [1696] 跳跃游戏 VI
 */

// @lc code=start
class Solution
{
public:
    int maxResult(vector<int> &nums, int k)
    {
        // 数组里面元素的个数
        const int N = nums.size();

        // get[]数组，表示每个位置能够获得的金币数量
        std::vector<int> get(N, 0);

        // 单调队列，注意，这里并不是严格单调
        deque<int> Q;

        for (int i = 0; i < N; i++)
        {
            // 在取最大值之前，需要保证单调队列中都是有效值。
            // 也就是都在区间里面的值
            // 当要求get[i]的时候，
            // 单调队列中应该是只能保存
            if (i - k > 0)
            {
                if (!Q.empty() && Q.front() == get[i - k - 1])
                {
                    Q.pop_front();
                }
            }

            // 从单调队列中取得较大值
            int old = Q.empty() ? 0 : Q.front();
            get[i] = old + nums[i];

            // 入队的时候，采用单调队列入队
            while (!Q.empty() && Q.back() < get[i])
            {
                Q.pop_back();
            }
            Q.push_back(get[i]);
        }

        return get[N - 1];
    }
};
// @lc code=end
