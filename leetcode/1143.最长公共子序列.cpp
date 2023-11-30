/*
 * @lc app=leetcode.cn id=1143 lang=cpp
 *
 * [1143] 最长公共子序列
 */

// @lc code=start
class Solution
{
public:
    int longestCommonSubsequence(string text1, string text2)
    {
        int m = text2.size();
        std::vector<int> dp(m + 1, 0);

        for (const auto &c : text1)
        {
            for (int j = 0, pre = 0; j < m; ++j)
            {
                auto tmp = dp[j + 1];
                dp[j + 1] = text2[j] == c ? pre + 1 : std::max(dp[j], dp[j + 1]);
                pre = tmp;
            }
        }

        return dp[m];
    }
};
// @lc code=end
