/*
 * @lc app=leetcode.cn id=384 lang=cpp
 *
 * [384] 打乱数组
 */

// @lc code=start
class Solution
{
public:
    Solution(vector<int> &nums) : origin(nums), shuff(nums)
    {
    }

    vector<int> reset()
    {
        return origin;
    }

    vector<int> shuffle()
    {
        int n = shuff.size();
        int count = n / 10 + 1;
        while (count-- > 0)
        {
            std::swap(shuff[rand() % n], shuff[rand() % n]);
        }
        return shuff;
    }

private:
    vector<int> origin;
    vector<int> shuff;
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(nums);
 * vector<int> param_1 = obj->reset();
 * vector<int> param_2 = obj->shuffle();
 */
// @lc code=end
