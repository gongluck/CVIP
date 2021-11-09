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
        for (int i = 1; i < n+1; ++i)
        {
            res = std::min(res,
                           std::max(
                               dp(k - 1, i - 1),
                               dp(k, n - i))) +
                  1;
        }

        return res;
    }
    int superEggDrop(int k, int n)
    {
        return dp(k, n);
    }
};
// @lc code=end
