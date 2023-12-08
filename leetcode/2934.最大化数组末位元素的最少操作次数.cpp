/*
 * @lc app=leetcode.cn id=2934 lang=cpp
 *
 * [2934] 最大化数组末位元素的最少操作次数
 */

// @lc code=start
class Solution
{
public:
    int operation(const std::vector<int> &nums1, const std::vector<int> &nums2, int last1, int last2)
    {
        int result = 0;
        int n = nums1.size();

        for (int i = 0; i < n - 1; ++i)
        {
            if (nums1[i] > last1 || nums2[i] > last2)
            {
                if (nums1[i] > last2 || nums2[i] > last1)
                {
                    return n;
                }

                // std::cout << "swap: " << i << std::endl;
                ++result;
            }
        }

        return result;
    }

    int minOperations(vector<int> &nums1, vector<int> &nums2)
    {
        int result1 = 0;
        int result2 = 0;
        int n = nums1.size();

        int result = std::min(operation(nums1, nums2, nums1[n - 1], nums2[n - 1]), 1 + operation(nums1, nums2, nums2[n - 1], nums1[n - 1]));

        return result >= n ? -1 : result;
    }
};
// @lc code=end
