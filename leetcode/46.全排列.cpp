/*
 * @lc app=leetcode.cn id=46 lang=cpp
 *
 * [46] 全排列
 */

// @lc code=start
class Solution
{
public:
    std::vector<std::vector<int>> res;
    void dfs(const std::vector<int> &nums, std::vector<int> order)
    {
        //决策终点
        if (nums.size() == order.size())
        {
            //添加到结果集
            res.push_back(order);
            return;
        }

        //遍历选择
        for (const auto &i : nums)
        {
            if (std::find(order.begin(), order.end(), i) != order.end())
            {
                //重复选择
                continue;
            }

            //选择
            order.push_back(i);
            //深度递进
            dfs(nums, order);
            //取消选择
            order.pop_back();
        }
    }
    vector<vector<int>> permute(vector<int> &nums)
    {
        dfs(nums, {});
        return res;
    }
};
// @lc code=end
