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
    std::vector<std::vector<int>> result;
    int n = nums.size();

    std::sort(nums.begin(), nums.end());
    for (int i = 0; i < n - 2; ++i)
    {
      if (nums[i] > 0)
      {
        break;
      }

      int left = i + 1;
      int right = n - 1;
      while (left < right)
      {
        int sums = nums[i] + nums[left] + nums[right];
        if (sums == 0)
        {
          result.emplace_back(std::vector<int>{nums[i], nums[left], nums[right]});
          while (left < right && nums[left] == nums[left + 1])
          {
            ++left;
          }
          ++left;
          while (left < right && nums[right] == nums[right - 1])
          {
            --right;
          }
          --right;
        }
        else if (sums > 0)
        {
          --right;
        }
        else
        {
          ++left;
        }
      }

      while (i < n - 2 && nums[i] == nums[i + 1])
      {
        ++i;
      }
    }

    return result;
  }
};
// @lc code=end
