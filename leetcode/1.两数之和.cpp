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
        std::unordered_map<int, int> visited;
        for (int i = 0; i < nums.size(); ++i)
        {
            if (visited.count(target - nums[i]) > 0)
            {
                return {visited[target - nums[i]], i};
            }
            else
            {
                visited.insert({nums[i], i});
            }
        }
        return {-1, -1};
    }
};
// @lc code=end
