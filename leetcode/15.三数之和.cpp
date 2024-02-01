/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution
{
public:
  vector<vector<int>> threeSum(vector<int> &nums)
  {
    vector<vector<int>> result;
    int len = nums.size();

    std::sort(nums.begin(), nums.end());

    for (int i = 0; i < len - 2; ++i)
    {
      if (nums[i] > 0)
      {
        break;
      }
      if (i > 0 && nums[i] == nums[i - 1])
      {
        continue;
      }

      int j = i + 1;
      int k = len - 1;
      while (j < k)
      {
        auto sum = nums[i] + nums[j] + nums[k];
        if (sum == 0)
        {
          result.push_back({nums[i], nums[j], nums[k]});
          for (++j; j < k && nums[j] == nums[j - 1]; ++j)
          {
          }
          for (--k; j < k && nums[k] == nums[k + 1]; --k)
          {
          }
        }
        else if (sum < 0)
        {
          ++j;
        }
        else
        {
          --k;
        }
      }
    }

    return result;
  }
};
// @lc code=end
