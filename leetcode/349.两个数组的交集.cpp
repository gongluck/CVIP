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
        std::vector<int> result;
        bool exist[10001] = {false};

        for (const auto &i : nums1)
        {
            exist[i] = true;
        }
        for (const auto &i : nums2)
        {
            if (exist[i])
            {
                result.push_back(i);
                exist[i] = false;
            }
        }

        return result;
    }
};
// @lc code=end
