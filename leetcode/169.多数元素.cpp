/*
 * @lc app=leetcode.cn id=169 lang=cpp
 *
 * [169] 多数元素
 */

// @lc code=start
class Solution
{
public:
    int majorityElement(vector<int> &nums)
    {
        // int n = nums.size();
        // n /= 2;

        // std::unordered_map<int, int> counts;
        // for (const auto &i : nums)
        // {
        //     if (++counts[i] > n)
        //     {
        //         return i;
        //     }
        // }

        // return 0; //!

        int n = nums.size();
        n /= 2;
        int candidate = 0;
        int count = 1;
        for (const auto &i : nums)
        {
            if (i == candidate)
            {
                if (++count > n)
                {
                    break;
                }
            }
            else if (--count == 0)
            {
                candidate = i;
                ++count; // 1
            }
        }

        return candidate;
    }
};
// @lc code=end
