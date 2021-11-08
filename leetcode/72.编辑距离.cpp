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
        std::vector<std::vector<int>> dp(word2.size() + 1, std::vector(word1.size() + 1, 0));

        for(int i = 0; i<word2.size()+1; ++i)
        {
            dp[i][0] = i;
        }
        for(int i = 0; i<word1.size()+1; ++i)
        {
            dp[0][i] = i;
        }

        for (int i = 0; i < word2.size(); ++i)
        {
            for (int j = 0; j < word1.size(); ++j)
            {
                if (word1[j] == word2[i])
                {
                    dp[i + 1][j + 1] = dp[i][j];
                }
                else
                {
                    dp[i + 1][j + 1] = std::min(dp[i][j], std::min(dp[i+1][j], dp[i][j+1])) + 1;
                }
            }
        }

        return dp[word2.size()][word1.size()];
    }
};
// @lc code=end
