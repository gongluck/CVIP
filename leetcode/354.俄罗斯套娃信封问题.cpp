/*
 * @lc app=leetcode.cn id=354 lang=cpp
 *
 * [354] 俄罗斯套娃信封问题
 */

// @lc code=start
class Solution
{
public:
    int maxEnvelopes(vector<vector<int>> &envelopes)
    {
        std::sort(envelopes.begin(), envelopes.end(),
                  [](const vector<int> &l, const vector<int> &r)
                  {
                      return (l[0] == r[0]) ? (l[1] < r[1]) : (l[0] < r[0]);
                  });

        std::vector<int> dp(envelopes.size(), 1);
        int res = 1;
        for (int i = 1; i < envelopes.size(); ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (envelopes[i][0] > envelopes[j][0] && envelopes[i][1] > envelopes[j][1])
                {
                    dp[i] = std::max(dp[i], dp[j] + 1);
                    res = std::max(res, dp[i]);
                }
            }
        }

        return res;
    }
};
// @lc code=end
