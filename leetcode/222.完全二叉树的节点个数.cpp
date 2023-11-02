/*
 * @lc app=leetcode.cn id=222 lang=cpp
 *
 * [222] 完全二叉树的节点个数
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
    int getheight(TreeNode *root)
    {
        int n = 0;
        while (root != nullptr)
        {
            ++n;
            root = root->left;
        }
        return n;
    }

    int countNodes(TreeNode *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        auto hleft = getheight(root->left);
        auto hright = getheight(root->right);

        if (hleft == hright)
        {
            return (1 << hleft) + countNodes(root->right);
        }
        else
        {
            return countNodes(root->left) + (1 << hright);
        }
    }
};
// @lc code=end
