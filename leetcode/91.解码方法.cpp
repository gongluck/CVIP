/*
 * @lc app=leetcode.cn id=91 lang=cpp
 *
 * [91] 解码方法
 */

// @lc code=start
class Solution
{
public:
    int numDecodings(string s)
    {
        int n = s.size();
        std::vector<int> dp(n + 1);
        dp[0] = 1;
        for (int i = 1; i <= n; ++i)
        {
            if (s[i - 1] != '0')
            {
                dp[i] = dp[i - 1];
            }
            if (i >= 2)
            {
                int t = (s[i - 2] - '0') * 10 + s[i - 1] - '0';
                if (t >= 10 && t <= 26)
                {
                    dp[i] += dp[i - 2];
                }
            }
        }
        return dp[n];
    }
};
// @lc code=end
