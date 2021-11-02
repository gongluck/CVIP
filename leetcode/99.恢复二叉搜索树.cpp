/*
 * @lc app=leetcode.cn id=99 lang=cpp
 *
 * [99] 恢复二叉搜索树
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
    TreeNode *pre = nullptr;
    TreeNode *n1 = nullptr, *n2 = nullptr;
    void dsf(TreeNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        dsf(root->left);

        if (pre && pre->val > root->val)
        {
            n2 = root;
            if (n1 == nullptr)
            {
                n1 = pre;
            }
            else
            {
                return;
            }
        }

        pre = root;

        dsf(root->right);
    }
    void recoverTree(TreeNode *root)
    {
        dsf(root);
        std::swap(n1->val, n2->val);
    }
};
// @lc code=end
