/*
 * @lc app=leetcode.cn id=669 lang=cpp
 *
 * [669] 修剪二叉搜索树
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
    TreeNode *trimBST(TreeNode *root, int low, int high)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        if (root->val < low)
        {
            return trimBST(root->right, low, high);
        }
        else if (root->val > high)
        {
            return trimBST(root->left, low, high);
        }
        else
        {
            root->left = trimBST(root->left, low, high);
            root->right = trimBST(root->right, low, high);
        }

        return root;
    }
};
// @lc code=end
