/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        std::vector<int> result(2, -1);
        std::unordered_map<int, int> maps;
        int n = nums.size();

        for (int i = 0; i < n; ++i)
        {
            if (maps.count(target - nums[i]) > 0)
            {
                result[0] = maps[target - nums[i]];
                result[1] = i;
                break;
            }
            else
            {
                maps[nums[i]] = i;
            }
        }

        return result;
    }
};
// @lc code=end
