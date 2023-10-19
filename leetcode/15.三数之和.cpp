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
    std::sort(nums.begin(), nums.end());
    int n = nums.size();

    int first = 0;
    while (first < n - 2)
    {
      if (nums[first] > 0)
      {
        break;
      }

      int second = first + 1;
      int third = n - 1;
      while (second < third)
      {
        if (nums[first] + nums[second] + nums[third] > 0)
        {
          --third;
        }
        else if (nums[first] + nums[second] + nums[third] < 0)
        {
          ++second;
        }
        else
        {
          result.push_back(std::vector<int>{nums[first], nums[second], nums[third]});
          while (++second < third && nums[second] == nums[second - 1])
          {
            continue;
          }
        }
      }

      while (++first < n - 2 && nums[first] == nums[first - 1])
      {
        continue;
      }
    }

    return result;
  }
};
// @lc code=end
