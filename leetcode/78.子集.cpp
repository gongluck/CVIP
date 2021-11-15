/*
 * @lc app=leetcode.cn id=78 lang=cpp
 *
 * [78] 子集
 */

// @lc code=start
class Solution
{
public:
    /*1*/
    // vector<vector<int>> subsets(vector<int> &nums)
    // {
    //     if (nums.size() <= 0)
    //     {
    //         return {{}};
    //     }

    //     auto num = nums.back();
    //     nums.pop_back();

    //     auto res = subsets(nums);
    //     auto size = res.size();
    //     for (int i = 0; i < size; ++i)
    //     {
    //         auto tmp = res[i];
    //         tmp.push_back(num);
    //         res.push_back(tmp);
    //     }

    //     return res;
    // }

    /*2*/
    vector<vector<int>> res;
    void dp(vector<int> &nums, int index, vector<int> &track)
    {
        res.push_back(track);
        if (index >= nums.size())
        {
            return;
        }

        for (int i = index; i < nums.size(); ++i)
        {
            track.push_back(nums[i]);
            dp(nums, i + 1, track);
            track.pop_back();
        }
    }
    vector<vector<int>> subsets(vector<int> &nums)
    {
        vector<int> track;
        dp(nums, 0, track);
        return res;
    }
};
// @lc code=end
