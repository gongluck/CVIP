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
        std::vector<std::vector<int>> dp(text2.size() + 1, std::vector<int>(text1.size() + 1, 0));

        for (int i = 0; i < text2.size(); ++i)
        {
            for (int j = 0; j < text1.size(); ++j)
            {
                if (text1[j] == text2[i])
                {
                    dp[i + 1][j + 1] = dp[i][j] + 1;
                }
                else
                {
                    dp[i+1][j+1] = std::max(dp[i][j+1], dp[i+1][j]);
                }
            }
        }

        return dp[text2.size()][text1.size()];
    }
};
// @lc code=end
