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
        std::unordered_map<int, int> hash;
        for (int i = 0; i < nums.size(); ++i)
        {
            auto found = hash.find(target - nums[i]);
            if (found != hash.end())
            {
                return {found->second, i};
            }
            hash.insert({nums[i], i});
        }
        return {-1, -1};
    }
};
// @lc code=end
