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
    //排序
    sort(nums.begin(), nums.end());
    for (int i = 0; i < nums.size(); i++)
    {
      //此循环第一个数大于0，那么肯定后面的都大于0，三个数的和肯定也大于0
      if (nums[i] > 0)
      {
        return result;
      }
      //此循环第一个数等于上一轮的第一个数，不会有新增结果集
      if (i > 0 && nums[i] == nums[i - 1])
      {
        continue;
      }
      //左右指针
      int left = i + 1;
      int right = nums.size() - 1;
      while (right > left)
      {
        //偏大
        if (nums[i] + nums[left] + nums[right] > 0)
        {
          right--;
        }
        //偏小
        else if (nums[i] + nums[left] + nums[right] < 0)
        {
          left++;
        }
        else
        {
          //添加结果集
          result.push_back(vector<int>{nums[i], nums[left], nums[right]});

          left++;
          while (right > left && nums[left] == nums[left - 1])
            left++;
          right--;
          while (right > left && nums[right] == nums[right + 1])
            right--;
        }
      }
    }
    return result;
  }
};
// @lc code=end
