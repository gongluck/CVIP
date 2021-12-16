/*
 * @lc app=leetcode.cn id=350 lang=cpp
 *
 * [350] 两个数组的交集 II
 */

// @lc code=start
class Solution
{
public:
    vector<int> intersect(vector<int> &nums1, vector<int> &nums2)
    {
        std::unordered_map<int, int> m1;
        for (const auto &i : nums1)
        {
            ++m1[i];
        }
        std::unordered_map<int, int> m2;
        for (const auto &i : nums2)
        {
            ++m2[i];
        }

        std::vector<int> res;
        for (const auto &e : m1)
        {
            if (m2.count(e.first) > 0)
            {
                auto n = std::min(e.second, m2[e.first]);
                while (n-- > 0)
                {
                    res.push_back(e.first);
                }
            }
        }

        return res;
    }
};
// @lc code=end
