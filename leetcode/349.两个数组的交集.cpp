/*
 * @lc app=leetcode.cn id=349 lang=cpp
 *
 * [349] 两个数组的交集
 */

// @lc code=start
class Solution
{
public:
    vector<int> intersection(vector<int> &nums1, vector<int> &nums2)
    {
        std::unordered_set<int> s1(nums1.begin(), nums1.end());
        std::unordered_set<int> both;
        for (const auto &i : nums2)
        {
            if (s1.count(i) > 0)
            {
                both.insert(i);
            }
        }
        std::vector<int> res(both.begin(), both.end());
        return res;
    }
};
// @lc code=end
