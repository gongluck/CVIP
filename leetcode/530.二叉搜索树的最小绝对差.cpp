/*
 * @lc app=leetcode.cn id=530 lang=cpp
 *
 * [530] 二叉搜索树的最小绝对差
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
    int difference(TreeNode *root, TreeNode *&pre)
    {
        if (root == nullptr)
        {
            return INT_MAX;
        }

        auto result = difference(root->left, pre);
        if (pre != nullptr)
        {
            result = std::min(result, std::abs(root->val - pre->val));
        }
        pre = root;
        return std::min(result, difference(root->right, pre));
    }

    int getMinimumDifference(TreeNode *root)
    {
        TreeNode *pre = nullptr;
        return difference(root, pre);
    }
};
// @lc code=end
