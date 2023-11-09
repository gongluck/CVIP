/*
 * @lc app=leetcode.cn id=108 lang=cpp
 *
 * [108] 将有序数组转换为二叉搜索树
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution
{
public:
    TreeNode *sort(const std::vector<int> &nums, int left, int right)
    {
        if (left >= right)
        {
            return nullptr;
        }

        auto mid = (left + right) / 2;
        auto node = new TreeNode(nums[mid]);
        node->left = sort(nums, left, mid);
        node->right = sort(nums, mid + 1, right);

        return node;
    }
    TreeNode *sortedArrayToBST(vector<int> &nums)
    {
        return sort(nums, 0, nums.size());
    }
};
// @lc code=end
