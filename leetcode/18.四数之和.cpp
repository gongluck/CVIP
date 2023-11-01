/*
 * @lc app=leetcode.cn id=18 lang=cpp
 *
 * [18] 四数之和
 */

// @lc code=start
class Solution
{
public:
    vector<vector<int>> fourSum(vector<int> &nums, int target)
    {
        std::vector<std::vector<int>> result;
        int n = nums.size();

        std::sort(nums.begin(), nums.end());
        for (int first = 0; first < n - 3; ++first)
        {
            if (first > 0 && nums[first] == nums[first - 1])
            {
                continue;
            }

            for (int second = first + 1; second < n - 2; ++second)
            {
                if (second > first + 1 && nums[second] == nums[second - 1])
                {
                    continue;
                }

                int third = second + 1;
                int fourth = n - 1;
                while (third < fourth)
                {
                    long long sums = (long long)nums[first] + nums[second] + nums[third] + nums[fourth];
                    if (sums > target)
                    {
                        --fourth;
                    }
                    else if (sums < target)
                    {
                        ++third;
                    }
                    else
                    {
                        result.emplace_back(std::vector<int>{nums[first], nums[second], nums[third], nums[fourth]});
                        ++third;
                        while (third < fourth && nums[third] == nums[third - 1])
                        {
                            ++third;
                        }
                        --fourth;
                        while (third < fourth && nums[fourth] == nums[fourth + 1])
                        {
                            --fourth;
                        }
                    }
                }
            }
        }

        return result;
    }
};
// @lc code=end
