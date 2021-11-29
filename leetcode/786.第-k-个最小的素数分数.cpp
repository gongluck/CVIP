/*
 * @lc app=leetcode.cn id=786 lang=cpp
 *
 * [786] 第 K 个最小的素数分数
 */

// @lc code=start
class Solution
{
public:
    vector<int> kthSmallestPrimeFraction(vector<int> &arr, int k)
    {
        vector<std::pair<int, int>> nums;
        for (int i = 0; i < arr.size() - 1; ++i)
        {
            for (int j = i + 1; j < arr.size(); ++j)
            {
                nums.push_back({arr[i], arr[j]});
            }
        }
        std::sort(nums.begin(), nums.end(), [](std::pair<int, int> a, std::pair<int, int> b)
                  {
                      double l = (double)a.first / (double)a.second;
                      double r = (double)b.first / (double)b.second;
                      return l < r ? 1 : 0;
                  });
        return {nums[k - 1].first, nums[k - 1].second};
    }
};
// @lc code=end
