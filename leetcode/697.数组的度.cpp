/*
 * @lc app=leetcode.cn id=697 lang=cpp
 *
 * [697] 数组的度
 */

// @lc code=start
class Solution
{
public:
    int findShortestSubArray(vector<int> &nums)
    {
        int result = INT_MAX;
        size_t maxd = 0;
        std::unordered_map<int, std::vector<int>> ns;

        for (int i = 0; i < nums.size(); ++i)
        {
            ns[nums[i]].push_back(i);
            // std::cout << "push: " << i << ", " << nums[i] << std::endl;
            maxd = std::max(maxd, ns[nums[i]].size());
        }
        // std::cout << maxd << std::endl;
        for (const auto &vec : ns)
        {
            if (vec.second.size() == maxd)
            {
                result = std::min(result, vec.second.back() - vec.second.front() + 1);
            }
        }

        return result;
    }
};
// @lc code=end
