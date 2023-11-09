/*
 * @lc app=leetcode.cn id=538 lang=cpp
 *
 * [538] 把二叉搜索树转换为累加树
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
    void convert(TreeNode *root, int &pre)
    {
        if (root == nullptr)
        {
            return;
        }

        convert(root->right, pre);
        root->val += pre;
        pre = root->val;
        convert(root->left, pre);

        return;
    }

    TreeNode *convertBST(TreeNode *root)
    {
        int pre = 0;
        convert(root, pre);
        return root;
    }
};
// @lc code=end
