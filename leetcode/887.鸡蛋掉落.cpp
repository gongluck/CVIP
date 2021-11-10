/*
 * @lc app=leetcode.cn id=887 lang=cpp
 *
 * [887] 鸡蛋掉落
 */

// @lc code=start
class Solution
{
public:
    std::unordered_map<std::string, int> visited;
    int dp(int k, int n)
    {
        if (k == 1)
        {
            return n;
        }
        if (n == 0)
        {
            return 0;
        }

        std::string key = std::to_string(k) + "," + std::to_string(n);
        if (visited.count(key) > 0)
        {
            return visited[key];
        }

        int res = INT_MAX;
        for (int i = 1; i < n + 1; ++i)
        {
            res = std::min(res,
                           std::max(
                               dp(k, n - i),
                               dp(k - 1, i - 1)) +
                               1);
        }

        visited[key] = res;
        return res;
    }
    int superEggDrop(int k, int n)
    {
        //return dp(k, n);

        std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n + 1, 0));

        int res = 0;
        while (dp[k][res] < n)
        {
            ++res;
            for (int i = 1; i <= k; ++i)
            {
                dp[i][res] = dp[i][res - 1] + dp[i - 1][res - 1] + 1;
            }
        }

        return res;
    }
};
// @lc code=end
