/*
 * @lc app=leetcode.cn id=450 lang=cpp
 *
 * [450] 删除二叉搜索树中的节点
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
    TreeNode *deleteNode(TreeNode *root, int key)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        TreeNode *result = root;
        if (root->val == key)
        {
            if (root->left == nullptr)
            {
                result = root->right;
            }
            else if (root->right == nullptr)
            {
                result = root->left;
            }
            else
            {
                auto rleftest = root->right;
                while (rleftest->left != nullptr)
                {
                    rleftest = rleftest->left;
                }
                rleftest->left = root->left;
                result = root->right;
            }
            delete root;
            return result;
        }
        else if (root->val > key)
        {
            root->left = deleteNode(root->left, key);
        }
        else
        {
            root->right = deleteNode(root->right, key);
        }

        return result;
    }
};
// @lc code=end
