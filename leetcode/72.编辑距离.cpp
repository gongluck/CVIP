/*
 * @lc app=leetcode.cn id=72 lang=cpp
 *
 * [72] 编辑距离
 */

// @lc code=start
class Solution
{
public:
    int minDistance(string word1, string word2)
    {
        int m = word1.size();
        int n = word2.size();
        std::vector<int> dp(n + 1, 0);

        for (int i = 0; i <= n; ++i)
        {
            dp[i] = i;
        }

        for (int i = 1; i <= m; ++i)
        {
            int leftup = dp[0]; // dp[i-1,j-1]
            dp[0] = i;
            for (int j = 1; j <= n; ++j)
            {
                int tmp = dp[j];
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[j] = leftup;
                }
                else
                {
                    dp[j] = std::min(dp[j], std::min(dp[j - 1], leftup)) + 1;
                }
                leftup = tmp;
            }
        }

        return dp[n];
    }
};
// @lc code=end
